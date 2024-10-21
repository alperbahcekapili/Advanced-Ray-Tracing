// #include "src/models/Triangle.h"
// #include "src/models/Camera.h"
// #include "src/models/ImagePane.h"
// #include "src/models/Ray.h"
// #include "src/models/Sphere.h"
// #include "src/lights/Light.h"
// #include "src/lights/PointLight.h"
// #include "src/scene/Scene.h"
// #include "src/shaders/Shader.h"
// #include "src/util/util.h"


// #include <vector>
// // for testing remove before submitting
// #include <opencv2/opencv.hpp>


// using namespace cv;


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

//     std::vector<float> image[imgWidth][imgHeight];
//     int fg_pixel_count = 0;
//     // now we need to iterate over the pixels and fill them
//     for (int i = 0; i < imgWidth; i++)
//     {
//         for (int j = 0; j < imgHeight; j++)
//         {

            
//             // shoot ray from camera to ImagePane
//             Ray cameraRay = imagePane->rayFromCamera(i, j);
//             // printf("Camera ray at: %d,%d: %f,%f,%f\n", i, j, cameraRay.d.at(0), cameraRay.d.at(1), cameraRay.d.at(2));
//             // now iterate over the objects to find first object that hits this ray
//             // add fov in future to exclude object that are too far away from the camera
//             float minTValue = 9999999;
//             int intersectingObjIndex = -1;
            
//             for (int k = 0; k < scene.numObjects; k++)
//             {
//                 float tvalue = objList[k]->Intersects(cameraRay);
//                 if (tvalue > 0 && tvalue < minTValue){
//                     minTValue = tvalue;
//                     intersectingObjIndex = k;
//                 }
//             }
        
//             if (minTValue > c->maxt || minTValue < c->mint){
//                 image[i][j] = {float(shader.scene->bg.at(0)),float(shader.scene->bg.at(1)),float(shader.scene->bg.at(2))};
//                 continue;
//             }
                

//             if (intersectingObjIndex == -1){
//                 image[i][j] = {float(shader.scene->bg.at(0)),float(shader.scene->bg.at(1)),float(shader.scene->bg.at(2))};
//                 continue;
//             }

            
            
//             vector<float> diffuse_intensity = {0,0,0};
//             // shader.diffuseShadingAt(cameraRay.locationAtT(minTValue), objList[intersectingObjIndex], intersectingObjIndex);
//             vector<float> ambient_intensity = shader.ambientShadingAt(cameraRay.locationAtT(minTValue), objList[intersectingObjIndex], intersectingObjIndex);
//             vector<float> specular_intensity = shader.specularShadingAt(cameraRay, cameraRay.locationAtT(minTValue) ,objList[intersectingObjIndex], intersectingObjIndex);
//             // TODO: replace hardcoded max hops, specular reflection also calculates diffuse
//             vector<float> specular_reflection = shader.specularReflection(cameraRay, &scene ,objList[intersectingObjIndex], 3, intersectingObjIndex);
//             vector<float> refrac_transmission = shader.refractionTransmission(cameraRay, &scene, objList[intersectingObjIndex], 5, intersectingObjIndex, false);
//             vector<float> pixel_val = vectorAdd(specular_intensity, vectorAdd(diffuse_intensity, ambient_intensity));
            
//             pixel_val = vectorAdd(specular_reflection, pixel_val);
//             //pixel_val = vectorAdd(refrac_transmission, pixel_val);
//             // std::cout << "Ambient Intensity: \n" <<  ambient_intensity.at(0) << ", " << ambient_intensity.at(1) << ", " << ambient_intensity.at(2) << "\n";
//             // std::cout << "Diffuse Intensity: \n" << diffuse_intensity.at(0) << ", " << diffuse_intensity.at(1) << ", " << diffuse_intensity.at(2) << "\n";
//             // std::cout << "Specular Intensity: \n" << specular_intensity.at(0) << ", " << specular_intensity.at(1) << ", " << specular_intensity.at(2) << "\n";
//             // std::cout << "Specular Reflectance: \n" << specular_reflection.at(0) << ", " << specular_reflection.at(1) << ", " << specular_reflection.at(2) << "\n";
//             std::cout << "refrac_transmission: \n" << refrac_transmission.at(0) << ", " << refrac_transmission.at(1) << ", " << refrac_transmission.at(2) << "\n";
            
                
//             image[i][j] = clipValues(pixel_val, 255.0);
//             // std::cout << image[i][j].at(0) << ", " << image[i][j].at(1) << ", " << image[i][j].at(2) << "\n";
//             fg_pixel_count++;
            
//         }
        
//     }
//     std::cout << "Outside the loop\n";
    

//     // Create an empty image with 3 channels (BGR) and set it to a specific color
//     cv::Mat imageMat(imgHeight, imgWidth, CV_32FC3); // Red color in BGR format


//     // Fill the Mat with the values from the float list
//     for (int i = 0; i < imgHeight; ++i) {
//         for (int j = 0; j < imgWidth; ++j) {
//             // std::cout << i << "," << j << "\n";
//             Vec3f intensity = imageMat.at<Vec3f>(Point(i,j));
//             intensity.val[0] = image[i][j].at(2);
//             intensity.val[1] = image[i][j].at(1);
//             intensity.val[2] = image[i][j].at(0);
//             imageMat.at<Vec3f>(Point(i,j)) = intensity;

//             // (cv::Scalar(image[i][j].at(0),image[i][j].at(1),image[i][j].at(2)));
//             // std::cout << image[i][j].at(0) << ", " << image[i][j].at(1) << ", " << image[i][j].at(2) << "\n";
//         }
//     }

//     printf("# of fg pixels: %d\n", fg_pixel_count);
//     cv::imshow("unNormalized Image", imageMat);
//     cv::waitKey(0);

//     // Normalize the image (min-max scaling to [0, 255])
//     cv::Mat normalizedImage;
//     double minVal, maxVal;
//     cv::minMaxLoc(imageMat.reshape(1), &minVal, &maxVal); // Reshape to single channel to find global min/max

//     // Scale and convert to 8-bit (CV_8UC3) for saving
//     imageMat.convertTo(normalizedImage, CV_8UC3, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

//     // Display the normalized image
    
//     cv::imwrite("output_image.png", normalizedImage);


    


// }

#include "src/util/parser.h"

int main(int argc, char const *argv[])
{
    std::string fp = "/Users/alperb/Advanced-Ray-Tracing/support_files/simple.xml";
    loadFromXml(fp);
    return 0;
}
