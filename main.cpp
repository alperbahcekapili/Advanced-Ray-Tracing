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


#include <vector>
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
        Scene curscene = *(scenes.at(i));
        Shader shader = Shader(
            scenes.at(i)
        );

        int imgWidth= curscene.imagePane->dimx;
        int imgHeight = curscene.imagePane->dimy;
        std::vector<std::vector<std::vector<float>>> image(imgWidth, std::vector<std::vector<float>>(imgHeight));
        // int fg_pixel_count = 0;
        // // now we need to iterate over the pixels and fill them
        int total_progress = 0;
        for (int i = 0; i < imgWidth; i++)
        {   if(total_progress % 10 == 0)
            printf("Totatl progress: %f\n", float(total_progress)/(imgHeight*imgWidth));

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
                int intersectingObjIndex = -1;
                
                for (int k = 0; k < curscene.numObjects; k++)
                {
                    float tvalue = curscene.sceneObjects[k]->Intersects(cameraRay);
                    // printf("Intersecing t val%f\n", tvalue);
                    if (tvalue > 0 && tvalue < minTValue){
                        minTValue = tvalue;
                        intersectingObjIndex = k;
                    }
                }
                // printf("Camera ray at: %d,%d: location: %f,%f,%f direction:%f,%f,%f\n", i, j, cameraRay.o.at(0), cameraRay.o.at(1), cameraRay.o.at(2) ,cameraRay.d.at(0), cameraRay.d.at(1), cameraRay.d.at(2));
            
                if (minTValue > curscene.camera->maxt){
                    // printf("Too far or too close\n");
                    // printf("%f",minTValue);
                    image[i][j] = {float(shader.scene->bg.at(0)),float(shader.scene->bg.at(1)),float(shader.scene->bg.at(2))};
                    continue;
                }
                    

                if (intersectingObjIndex == -1){
                    // printf("No intersections\n");
                    image[i][j] = {float(shader.scene->bg.at(0)),float(shader.scene->bg.at(1)),float(shader.scene->bg.at(2))};
                    continue;
                }

               // std::cout << i << "," << j << "\n";
                
                Object** objList = curscene.sceneObjects;
                vector<float> diffuse_intensity = shader.diffuseShadingAt(cameraRay.locationAtT(minTValue), objList[intersectingObjIndex], intersectingObjIndex);
                vector<float> ambient_intensity = shader.ambientShadingAt(cameraRay.locationAtT(minTValue), objList[intersectingObjIndex], intersectingObjIndex);
                vector<float> specular_intensity = {0,0,0}; shader.specularShadingAt(cameraRay, cameraRay.locationAtT(minTValue) ,objList[intersectingObjIndex], intersectingObjIndex);
                // TODO: replace hardcoded max hops, specular reflection also calculates diffuse
                
                vector<float> pixel_val = vectorAdd(specular_intensity, vectorAdd(diffuse_intensity, ambient_intensity));
                
                // if(objList[intersectingObjIndex]->getMaterial()->materialType == MaterialType::Mirror){
                //     vector<float> specular_reflection = shader.specularReflection(cameraRay, &curscene ,objList[intersectingObjIndex], 6, intersectingObjIndex);
                //     pixel_val = vectorAdd(specular_reflection, pixel_val);
                // }
                // else if (objList[intersectingObjIndex]->getMaterial()->materialType == MaterialType::Dielectric || objList[intersectingObjIndex]->getMaterial()->materialType == MaterialType::Conductor){
                //     vector<float> refrac_transmission = shader.refractionTransmission(cameraRay, &curscene, objList[intersectingObjIndex], 6, intersectingObjIndex);
                //     pixel_val = vectorAdd(refrac_transmission, pixel_val);
                //     // std::cout << "refrac_transmission: \n" << refrac_transmission.at(0) << ", " << refrac_transmission.at(1) << ", " << refrac_transmission.at(2) << "\n";

                // }

                //pixel_val = vectorAdd(refrac_transmission, pixel_val);
                // std::cout << "Ambient Intensity: \n" <<  ambient_intensity.at(0) << ", " << ambient_intensity.at(1) << ", " << ambient_intensity.at(2) << "\n";
                // std::cout << "Diffuse Intensity: \n" << diffuse_intensity.at(0) << ", " << diffuse_intensity.at(1) << ", " << diffuse_intensity.at(2) << "\n";
                // std::cout << "Specular Intensity: \n" << specular_intensity.at(0) << ", " << specular_intensity.at(1) << ", " << specular_intensity.at(2) << "\n";
                // std::cout << "Specular Reflectance: \n" << specular_reflection.at(0) << ", " << specular_reflection.at(1) << ", " << specular_reflection.at(2) << "\n";
                
                
                    
                image[i][j] = clipValues(pixel_val, 255.0);
                // std::cout << image[i][j].at(0) << ", " << image[i][j].at(1) << ", " << image[i][j].at(2) << "\n";

                // fg_pixel_count++;
                
            }
            
        }
        // std::cout << "Outside the loop\n";
        

        


        unsigned width = imgWidth, height = imgHeight;
        std::vector<unsigned char> image_mat(width * height * 4, 255); // White image

        // Set some pixels to black
        for (int y = 0; y < imgWidth; ++y) {
            for (int x = 0; x < imgHeight; ++x) {
                int idx = 4 * (y * width + x);
                image_mat[idx + 0] = static_cast<uint8_t>(clamp(image[x][y][0], 0.0f, 255.0f));
                image_mat[idx + 1] = static_cast<uint8_t>(clamp(image[x][y][1], 0.0f, 255.0f)); 
                image_mat[idx + 2] = static_cast<uint8_t>(clamp(image[x][y][2], 0.0f, 255.0f)); 
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
