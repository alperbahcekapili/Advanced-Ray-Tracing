#include "src/models/Triangle.h"
#include "src/models/Camera.h"
#include "src/models/ImagePane.h"
#include "src/models/Ray.h"
#include "src/models/Sphere.h"
#include "src/lights/Light.h"
#include "src/lights/PointLight.h"
#include "src/scene/Scene.h"
#include "src/shaders/Shader.h"
#include "src/util/util.h"
#include "src/util/lodepng.h"
#include "src/util/parser.h"
#include "src/acceleration/BVH.h"

#include <algorithm>



#include <cmath> // For fmod
#include "src/models/Material.h"
#include <stdio.h>




int main(int argc, char const *argv[])
{
    if(argc<1)
        {printf("Please give a xml file...");
            return -1;}
    std::string fp = std::string(argv[1]);

    std::cout << "File path: " << fp << std::endl;

    std::vector<Scene*> scenes = loadFromXml(fp);


    std::cout << "xml loaded\n";
    for (size_t i = 0; i < scenes.size(); i++)
    {

        BVH* bvh = new BVH(scenes.at(i)->sceneObjects, scenes.at(i)->numObjects, 0);
        for (int j = 0; j < scenes.at(i)->numObjects; j++)
        {
            scenes.at(i)->sceneObjects[j]->id = j;
        }
        // bvh->visualize(0);
        scenes.at(i)->bvh = bvh;

        // we also need to define 8 vectors for each of the corners of BVH to use in perlin noise texture


        Scene curscene = *(scenes.at(i));
        Shader shader = Shader(
            scenes.at(i)
        );

        int imgWidth= curscene.imagePane->dimx;
        int imgHeight = curscene.imagePane->dimy;
        Vec3** image = new Vec3*[imgWidth];
        // int fg_pixel_count = 0;
        // // now we need to iterate over the pixels and fill them
        int total_progress = 0;
        // #pragma omp parallel for
        for (int i = 0; i < imgWidth; i++)
        {
            image[i] = new Vec3[imgHeight];

            // printf("Totatl progress: %f\n", float(total_progress)/(imgHeight*imgWidth));

            for (int j = 0; j < imgHeight; j++)
            {

                total_progress++;
                
                if(i==500 && j==500)    
                 printf("Alper");
                
                // printf("Total Progress: %d/%d\n",total_progress, imgWidth*imgHeight);
                Vec3 cumulative_pixel = Vec3(0,0,0);
                for (int rayindex = 0; rayindex < curscene.camera->numsamples; rayindex++)
                {
                    // shoot ray from camera to ImagePane
                Ray cameraRay = curscene.imagePane->rayFromCamera(i, j, rayindex);

                // now iterate over the objects to find first object that hits this ray
                // add fov in future to exclude object that are too far away from the camera
                float minTValue = 9999999;
                float maxTValue = -1;


                Object* tofill = nullptr;
                bool intersected = bvh->intersectObject(cameraRay, tofill, minTValue, maxTValue);
                int intersectingObjIndex = -1;
                if(intersected)
                    intersectingObjIndex = tofill -> id;                    

                // printf("Camera ray at: %d,%d: location: %f,%f,%f direction:%f,%f,%f\n", i, j, cameraRay.o.x, cameraRay.o.y, cameraRay.o.z ,cameraRay.d.x, cameraRay.d.y, cameraRay.d.z);

                if (intersectingObjIndex == -1){
                    // if scene has bg texture then use it
                    Vec3 bg_pixel_val = Vec3(float(shader.scene->bg.x),float(shader.scene->bg.y),float(shader.scene->bg.z));
                    if(shader.scene->bg_texture_set){
                        // if i < imgwidth/2 and j < imgheight/2 then we are on the topleft triangle
                        bool topleft = false;
                        if(i < imgWidth/2  && j < imgHeight/2)
                            topleft = true;
                        // intersection point is defined by pixel indices
                        Vec3 intersection_point = Vec3(i, j, 1);
                        Vec3 v1 = topleft ? Vec3(0,0,0) : Vec3(imgWidth, imgHeight, 1);
                        Vec3 v2 = Vec3(0, imgHeight, 1);
                        Vec3 v3 = Vec3(imgWidth, 0, 1);
                        std::pair<double, double> uv_a = {0, 0}, uv_b = {1, 0}, uv_c = {0, 1};
                        if(! topleft)
                            uv_a = {1,1};
                        uv tmp_uv = uv::calculateUVTriangle(intersection_point, v1, v2, v3, uv_a, uv_b, uv_c);
                        bg_pixel_val = shader.scene->bg_texture->interpolateAt(tmp_uv, NEAREAST_NEIGHBOR) * 255 ;
                        
                    }
                    if(shader.scene->spherical_light_flag){
                        
                        bg_pixel_val = shader.scene->spherical_light->irradianceAt(cameraRay, Vec3(-1,-1,-1));

                    }

                    cumulative_pixel = cumulative_pixel +  (bg_pixel_val/curscene.camera->numsamples);
                    image[i][j] = clipValues(cumulative_pixel, 255.0);
                    continue;
                }
            
                
                Vec3  diffuse_intensity = shader.diffuseShadingAt(cameraRay.locationAtT(minTValue), tofill, intersectingObjIndex);
                Vec3  ambient_intensity = shader.ambientShadingAt(cameraRay.locationAtT(minTValue), tofill, intersectingObjIndex);
                Vec3  specular_intensity = shader.specularShadingAt(cameraRay, cameraRay.locationAtT(minTValue) , tofill, intersectingObjIndex);

                Vec3  pixel_val = specular_intensity + diffuse_intensity + ambient_intensity;
                

                if(tofill->getMaterial()->materialType == MaterialType::Mirror){
                    Vec3  specular_reflection = shader.specularReflection(cameraRay, &curscene ,tofill, 6, intersectingObjIndex);
                    pixel_val = specular_reflection + pixel_val;
                    // std::cout << "specular_reflection: \n" << specular_reflection.x << ", " << specular_reflection.y << ", " << specular_reflection.z << "\n";

                }
                else if (tofill->getMaterial()->materialType == MaterialType::Dielectric || tofill->getMaterial()->materialType == MaterialType::Conductor){
                    Vec3  refrac_transmission = shader.refractionTransmission(cameraRay, &curscene, tofill, 6, intersectingObjIndex);
                    pixel_val = refrac_transmission + pixel_val;
                    // std::cout << "refrac_transmission: \n" << refrac_transmission.x << ", " << refrac_transmission.y << ", " << refrac_transmission.z << "\n";

                }


                // std::cout << "Ambient Intensity: \n" <<  ambient_intensity.x << ", " << ambient_intensity.y << ", " << ambient_intensity.z << "\n";
                // std::cout << "Diffuse Intensity: \n" << diffuse_intensity.x << ", " << diffuse_intensity.y << ", " << diffuse_intensity.z << "\n";
                // std::cout << "Specular Intensity: \n" << specular_intensity.x << ", " << specular_intensity.y << ", " << specular_intensity.z << "\n";


                cumulative_pixel = cumulative_pixel + (pixel_val/curscene.camera->numsamples);
                
                // std::cout << image[i][j].x << ", " << image[i][j].y << ", " << image[i][j].z << "\n";

                
                }

                image[i][j] = cumulative_pixel;

            }

        }



        float* flatArray = new float[imgWidth * imgHeight * 3];
        convertVec3ToFlatArray(image, imgWidth, imgHeight, flatArray);

        

        

        unsigned width = imgWidth, height = imgHeight;
        std::vector<unsigned char> image_mat(width * height * 3, 255); // White image

        
        // Set some pixels to black
        for (int x = 0; x < imgWidth * imgHeight * 3; ++x) {
            image_mat[x] = static_cast<uint8_t>(flatArray[x]);
        }
                

                

        printf("Resmi olusturdum");
        size_t pos = curscene.camera->name.find(".exr");
        if (pos != std::string::npos) {
            curscene.camera->name.replace(pos, 4, ".png");  // Replace 4 characters (".exr")
        }
        // Encode the image and save to file
        unsigned error = lodepng::encode(curscene.camera->name, image_mat, width, height, LCT_RGB);

        // Check for encoding errors
        if (error) {
            std::cerr << "Error encoding PNG: " << lodepng_error_text(error) << std::endl;
        } else {
            std::cout << "Image saved successfully!" << std::endl;
        }



    }

    return 0;
}
