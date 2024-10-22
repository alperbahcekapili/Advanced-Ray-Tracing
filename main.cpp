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

#include "src/util/parser.h"


#include <vector>
#include <algorithm>
// for testing remove before submitting
#include <opencv2/opencv.hpp>


using namespace cv;

template <typename T>
T clamp(T val, T minVal, T maxVal) {
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return val;
}

// int main(){

//     // Define objects
//     float phong_exp =  10;;
//     MaterialType triangleMaterialType = MaterialType::Mirror;
//     float ambientProp[3] = {0.3,0.3,0.3};
//     float diffuse[3] = {1,0.2,1};
//     float specular[3] = {0.1,0.1,0.1};

//     Material* triangleMaterial = new Material(triangleMaterialType, ambientProp, diffuse, specular, phong_exp, {1,0.2,1}, 0.3, 0.3, 0.3) ;
//     float triangleVertices[3][3] = {{550,20,-400}, {0,0,-400}, {0,500,-500}};
//     Triangle* triangle = new Triangle(triangleMaterial, ObjectType::TriangleType, triangleVertices[0], triangleVertices[1], triangleVertices[2]);
//     float floorVertices[3][3] = {{-1000,-100,1000}, {0,-20,-1000}, {1000,-100,1000}};
//     Triangle* floor = new Triangle(triangleMaterial, ObjectType::TriangleType, floorVertices[0], floorVertices[1], floorVertices[2]);

//     float amb[3] = {0,0,0};
//     float shepere_dif[3] = {0,1, 0};
//     Material* sphereMaterial = new Material(MaterialType::Dielectric, amb, shepere_dif, specular, phong_exp, {1,1,1}, 0.1, 0.1, 0.1);
//     Sphere* sphere = new Sphere(
//         0, 0, -300, 50, sphereMaterial, ObjectType::SphereType
//     );


//     // now initialize scene and iteratively fill the ImagePane
//     Object** objList = new Object*[3];
//     std::cout << "I'll be setting triangle:..\n";
//     objList[0] = triangle;
//     objList[1] = sphere;
//     objList[2] = floor;
    
//     // Ray* test_ray = new Ray({8,8,-5}, {0,0,1});
//     // float test_t_val = triangle->Intersects(*test_ray);
//     // std::vector<float> tri_surface_n = triangle->getSurfaceNormal({8,8,0});
//     // std::cout << "Surface Normal: " << tri_surface_n.at(0) << ", " << tri_surface_n.at(1) << ", " << tri_surface_n.at(2) << "\n";
    

//     // intiialize camera
//     vector<float> u = {1,0,0};
//     vector<float> v = {0,1,0};
//     vector<float> e = {0,20,100};
//     Camera* c = new Camera(u, v, e, 0, 1000);

//     // Initialize Image pane
//     int imgHeight =200;
//     int imgWidth = 200;
//     ImagePane* imagePane = new ImagePane(
//         imgHeight, imgWidth, -imgHeight/2, imgWidth/2, -imgHeight/2, imgWidth/2, 350, c 
//     );


//     // Lights
//     int numlights = 2;
//     Light** lightList = new Light*[numlights];
//     std::vector<float> light_location = {150, 0, -300};
//     std::vector<float> light_intensity = {1500, 1500, 1500};
//     lightList[0] = new PointLight(light_intensity, light_location);
//     lightList[1] = new PointLight(light_intensity, {0, 0, -200});

//     // Scene
//     int bg[3] = {0,0,0};
//     int ambientLight[3] = {1,1,1};
//     float medium_refraction_index =  0.001;
//     Scene scene = Scene(
//         objList, 3, c, imagePane, lightList, numlights, bg, ambientLight, 0.0001, medium_refraction_index
//     );

//     std::cout << "Scene initialized\n";

//     // Shader
//     Shader shader = Shader(
//         &scene   
//     );

//     


    


// }

int main(int argc, char const *argv[])
{
    std::string fp = "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/cornellbox.xml";
    std::vector<Scene*> scenes = loadFromXml(fp);
    for (size_t i = 0; i < scenes.size(); i++)
    {   
        int scenei = i;
        Scene curscene = *(scenes.at(i));
        Shader shader = Shader(
            scenes.at(i)
        );

        int imgWidth= curscene.imagePane->dimx;
        int imgHeight = curscene.imagePane->dimy;
        std::vector<std::vector<std::vector<float>>> image(imgWidth, std::vector<std::vector<float>>(imgHeight));
        int fg_pixel_count = 0;
        // now we need to iterate over the pixels and fill them
        int total_progress = 0;
        for (int i = 0; i < imgWidth; i++)
        {
            for (int j = 0; j < imgHeight; j++)
            {

                total_progress++;
                if (total_progress % 10 == 0){
                    // printf("Scene: %d/%ld, Total Progress: %d\n", scenei, scenes.size(), total_progress);
                }
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

                
                Object** objList = curscene.sceneObjects;
                vector<float> diffuse_intensity = shader.diffuseShadingAt(cameraRay.locationAtT(minTValue), objList[intersectingObjIndex], intersectingObjIndex);
                vector<float> ambient_intensity = shader.ambientShadingAt(cameraRay.locationAtT(minTValue), objList[intersectingObjIndex], intersectingObjIndex);
                vector<float> specular_intensity = {0,0,0};
                // shader.specularShadingAt(cameraRay, cameraRay.locationAtT(minTValue) ,objList[intersectingObjIndex], intersectingObjIndex);
                // TODO: replace hardcoded max hops, specular reflection also calculates diffuse
                // vector<float> specular_reflection = shader.specularReflection(cameraRay, &curscene ,objList[intersectingObjIndex], 3, intersectingObjIndex);
                // vector<float> refrac_transmission = shader.refractionTransmission(cameraRay, &curscene, objList[intersectingObjIndex], 5, intersectingObjIndex, false);
                vector<float> pixel_val = vectorAdd(specular_intensity, vectorAdd(diffuse_intensity, ambient_intensity));
                
                // pixel_val = vectorAdd(specular_reflection, pixel_val);
                //pixel_val = vectorAdd(refrac_transmission, pixel_val);
                std::cout << "Ambient Intensity: \n" <<  ambient_intensity.at(0) << ", " << ambient_intensity.at(1) << ", " << ambient_intensity.at(2) << "\n";
                std::cout << "Diffuse Intensity: \n" << diffuse_intensity.at(0) << ", " << diffuse_intensity.at(1) << ", " << diffuse_intensity.at(2) << "\n";
                std::cout << "Specular Intensity: \n" << specular_intensity.at(0) << ", " << specular_intensity.at(1) << ", " << specular_intensity.at(2) << "\n";
                // std::cout << "Specular Reflectance: \n" << specular_reflection.at(0) << ", " << specular_reflection.at(1) << ", " << specular_reflection.at(2) << "\n";
                // std::cout << "refrac_transmission: \n" << refrac_transmission.at(0) << ", " << refrac_transmission.at(1) << ", " << refrac_transmission.at(2) << "\n";
                
                    
                image[i][j] = clipValues(pixel_val, 255.0);
                // std::cout << image[i][j].at(0) << ", " << image[i][j].at(1) << ", " << image[i][j].at(2) << "\n";

                fg_pixel_count++;
                
            }
            
        }
        std::cout << "Outside the loop\n";
        

        // Create an empty image with 3 channels (BGR) and set it to a specific color
        cv::Mat imageMat(imgHeight, imgWidth, CV_8UC3); // Red color in BGR format


        // Fill the Mat with the values from the float list
        for (int i = 0; i < imgHeight; ++i) {
            for (int j = 0; j < imgWidth; ++j) {
                
                imageMat.at<cv::Vec3b>(j, i)[0] = static_cast<uint8_t>(clamp(image[i][j][2], 0.0f, 255.0f)); // Blue channel
                imageMat.at<cv::Vec3b>(j, i)[1] = static_cast<uint8_t>(clamp(image[i][j][1], 0.0f, 255.0f)); // Green channel
                imageMat.at<cv::Vec3b>(j, i)[2] = static_cast<uint8_t>(clamp(image[i][j][0], 0.0f, 255.0f)); // Red channel (cv::Scalar(image[i][j].at(0),image[i][j].at(1),image[i][j].at(2)));
                // std::cout << image[i][j].at(0) << ", " << image[i][j].at(1) << ", " << image[i][j].at(2) << "\n";
            }
        }

        printf("# of fg pixels: %d\n", fg_pixel_count);
        cv::imshow("unNormalized Image", imageMat);
        cv::waitKey(0);

        // Normalize the image (min-max scaling to [0, 255])
       
        
        cv::imwrite("output_image.png", imageMat);

    }
    
    return 0;
}
