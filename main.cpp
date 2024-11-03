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


template <typename T>
T clamp(T val, T minVal, T maxVal) {
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return val;
}

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
        for (int i = 0; i < imgWidth; i++)
        {   
            image[i] = new Vec3[imgHeight];
            
            //  printf("Totatl progress: %f\n", float(total_progress)/(imgHeight*imgWidth));

            for (int j = 0; j < imgHeight; j++)
            {

            total_progress++;
                // if (total_progress % 10 == 0){
                //     // printf("Scene: %d/%ld, Total Progress: %d\n", scenei, scenes.size(), total_progress);
                // }
                // shoot ray from camera to ImagePane
                Ray cameraRay = curscene.imagePane->rayFromCamera(i, j);
                
                // now iterate over the objects to find first object that hits this ray
                // add fov in future to exclude object that are too far away from the camera
                float minTValue = 9999999;
                float maxTValue = -1;
                


                Object* tofill = nullptr;
                bool intersected = bvh->intersectObject(cameraRay, tofill, minTValue, maxTValue);
                if(!intersected)
                continue;
                int intersectingObjIndex = tofill -> id;
               

                // std::cout << intersectingObjIndex << "\n";

                

                // printf("Camera ray at: %d,%d: location: %f,%f,%f direction:%f,%f,%f\n", i, j, cameraRay.o.x, cameraRay.o.y, cameraRay.o.z ,cameraRay.d.x, cameraRay.d.y, cameraRay.d.z);
            
                if (minTValue > curscene.camera->maxt){
                    // printf("Too far or too close\n");
                    // printf("%f",minTValue);
                    image[i][j] = Vec3(float(shader.scene->bg.x),float(shader.scene->bg.y),float(shader.scene->bg.z));
                    continue;
                }
                    

                if (intersectingObjIndex == -1){
                    // printf("No intersections\n");
                    image[i][j] = Vec3(float(shader.scene->bg.x),float(shader.scene->bg.y),float(shader.scene->bg.z));
                    continue;
                }

               // std::cout << i << "," << j << "\n";
                
                Object** objList = curscene.sceneObjects;
                Vec3  diffuse_intensity = shader.diffuseShadingAt(cameraRay.locationAtT(minTValue), objList[intersectingObjIndex], intersectingObjIndex);
                Vec3  ambient_intensity = shader.ambientShadingAt(cameraRay.locationAtT(minTValue), objList[intersectingObjIndex], intersectingObjIndex);
                Vec3  specular_intensity = shader.specularShadingAt(cameraRay, cameraRay.locationAtT(minTValue) ,objList[intersectingObjIndex], intersectingObjIndex);
                // TODO: replace hardcoded max hops, specular reflection also calculates diffuse
                
                Vec3  pixel_val = specular_intensity + diffuse_intensity + ambient_intensity;
                
                if(objList[intersectingObjIndex]->getMaterial()->materialType == MaterialType::Mirror){
                    Vec3  specular_reflection = shader.specularReflection(cameraRay, &curscene ,objList[intersectingObjIndex], 6, intersectingObjIndex);
                    pixel_val = specular_reflection + pixel_val;
                    // std::cout << "Type is mirror \n";
                    // std::cout << "specular_reflection: \n" << specular_reflection.x << ", " << specular_reflection.y << ", " << specular_reflection.z << "\n";


                }
                else if (objList[intersectingObjIndex]->getMaterial()->materialType == MaterialType::Dielectric || objList[intersectingObjIndex]->getMaterial()->materialType == MaterialType::Conductor){
                    Vec3  refrac_transmission = shader.refractionTransmission(cameraRay, &curscene, objList[intersectingObjIndex], 6, intersectingObjIndex);
                    pixel_val = refrac_transmission + pixel_val;
                    // std::cout << "refrac_transmission: \n" << refrac_transmission.x << ", " << refrac_transmission.y << ", " << refrac_transmission.z << "\n";

                }

                
                // std::cout << "Ambient Intensity: \n" <<  ambient_intensity.x << ", " << ambient_intensity.y << ", " << ambient_intensity.z << "\n";
                // std::cout << "Diffuse Intensity: \n" << diffuse_intensity.x << ", " << diffuse_intensity.y << ", " << diffuse_intensity.z << "\n";
                // std::cout << "Specular Intensity: \n" << specular_intensity.x << ", " << specular_intensity.y << ", " << specular_intensity.z << "\n";
                // std::cout << "Specular Reflectance: \n" << specular_reflection.x << ", " << specular_reflection.y << ", " << specular_reflection.z << "\n";
                
                
                    
                image[i][j] = clipValues(pixel_val, 255.0);
                // std::cout << image[i][j].x << ", " << image[i][j].y << ", " << image[i][j].z << "\n";

                // fg_pixel_count++;
                
            }
            
        }
        // std::cout << "Outside the loop\n";
        

        


        unsigned width = imgWidth, height = imgHeight;
        std::vector<unsigned char> image_mat(width * height * 4, 255); // White image

        // Set some pixels to black
        for (int x = 0; x < imgWidth; ++x) {
            for (int y = 0; y < imgHeight; ++y) {
                int idx = 4 * (y * width + x);
                image_mat[idx + 0] = static_cast<uint8_t>(clamp(image[x][y].x, 0.0f, 255.0f));
                image_mat[idx + 1] = static_cast<uint8_t>(clamp(image[x][y].y, 0.0f, 255.0f)); 
                image_mat[idx + 2] = static_cast<uint8_t>(clamp(image[x][y].z, 0.0f, 255.0f)); 
                image_mat[idx + 3] = 255; // Alpha (opaque)
            }
        }

        // Encode the image and save to file
        unsigned error = lodepng::encode(curscene.camera->name, image_mat, width, height);

        // Check for encoding errors
        if (error) {
            std::cerr << "Error encoding PNG: " << lodepng_error_text(error) << std::endl;
        } else {
            std::cout << "Image saved successfully!" << std::endl;
        }



    }
    
    return 0;
}
