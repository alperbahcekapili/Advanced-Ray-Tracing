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


#include <vector>
// for testing remove before submitting
#include <opencv2/opencv.hpp>


using namespace cv;


int main(){
    
    vector<float> d;
    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    vector<float> o;
    o.push_back(0);
    o.push_back(0);
    o.push_back(0);

    Ray* ray = new Ray(o,d);
    cout << ray->toString();


    MaterialType triangleMaterialType = MaterialType::Mirror;
    float ambientProp[3] = {4,5,1};
    float diffuse = 1;
    float specular[3] = {6,7,1};
    Material* triangleMaterial = new Material(triangleMaterialType, ambientProp, diffuse, specular, {0,255,0});
    std::cout << triangleMaterial->diffuseProp;
    float triangleVertices[3][3] = {{150,20,-20}, {0,0,-20}, {0,500,-20}};
    Triangle* triangle = new Triangle(triangleMaterial, ObjectType::TriangleType, triangleVertices[0], triangleVertices[1], triangleVertices[2]);
    Ray* test_ray = new Ray({8,8,-5}, {0,0,1});
    float test_t_val = triangle->Intersects(*test_ray);
    // std::vector<float> tri_surface_n = triangle->getSurfaceNormal({8,8,0});
    // std::cout << "Surface Normal: " << tri_surface_n.at(0) << ", " << tri_surface_n.at(1) << ", " << tri_surface_n.at(2) << "\n";
    

    
    vector<float> u = {1,0,0};
    vector<float> v = {0,1,0};
    vector<float> e = {0,0,100};
    // intiialize camera
    Camera* c = new Camera(u, v, e);

    int imgHeight = 100;
    int imgWidth = 100;


    ImagePane* imagePane = new ImagePane(
        imgHeight, imgWidth, -imgHeight/2, imgWidth/2, -imgHeight/2, imgWidth/2, 104, c 
    );

    Material* sphereMaterial = new Material(MaterialType::Conductor, ambientProp, diffuse, specular, {255,0,0});
    Sphere* sphere = new Sphere(
        0, 0, -30, 25.9, sphereMaterial, ObjectType::SphereType
    );

    // now initialize scene and iteratively fill the ImagePane
    Object** objList = new Object*[2];
    std::cout << "I'll be setting triangle:..\n";
    objList[0] = triangle;
    objList[1] = sphere;
    
     // Debug: Print pointer addresses before accessing objList[0]->material
    std::cout << "sphere addre  ss: " << sphere << std::endl;
    std::cout << "objList[0] address: " << objList[0] << std::endl;
    std::cout << "sphere->material address: " << sphere->getObject() << std::endl;
    std::cout << "objList[0]->material address: " << objList[0]->getObject() << std::endl;

    Light** lightList = new Light*[1];
    std::vector<float> light_location = {0,0,0};
    lightList[0] = new PointLight(5, light_location);

    std::cout << "Center of the light: " << light_location.at(0) << "," << light_location.at(1) << "," << light_location.at(2) << "\n\n";

    std::cout << "Setting scene...\n";
    Scene scene = Scene(
        objList, 1, *c, *imagePane, lightList, 1, {0,0,0}
    );

    std::cout << "Scene initialized\n";

    Shader shader = Shader(
        &scene   
    );

    std::vector<float> image[imgWidth][imgHeight];
    int fg_pixel_count = 0;
    // now we need to iterate over the pixels and fill them
    for (int i = 0; i < imgWidth; i++)
    {
        for (int j = 0; j < imgHeight; j++)
        {

            
            // shoot ray from camera to ImagePane
            Ray cameraRay = imagePane->rayFromCamera(i, j);
            // printf("Camera ray at: %d,%d: %f,%f,%f\n", i, j, cameraRay.d.at(0), cameraRay.d.at(1), cameraRay.d.at(2));
            // now iterate over the objects to find first object that hits this ray
            // add fov in future to exclude object that are too far away from the camera
            float minTValue = 9999999;
            int intersectingObjIndex = -1;
            
            for (int k = 0; k < 2; k++)
            {
                float tvalue = objList[k]->Intersects(cameraRay);
                if (tvalue > 0 && tvalue < minTValue){
                    minTValue = tvalue;
                    intersectingObjIndex = k;
                }
            }

            if (intersectingObjIndex == -1){
                // means we need to set bg color for this pixel
                
                image[i][j] = shader.scene->bg;
                continue;
            }

            
            
            vector<float> pixel_val = shader.diffuseShadingAt(cameraRay.locationAtT(minTValue), objList[intersectingObjIndex], intersectingObjIndex);
            if( pixel_val.at(0) == 0 && pixel_val.at(1) == 0 && pixel_val.at(2) == 0){
                image[i][j] = shader.scene->bg;
                continue;
            }
                
            image[i][j] = clipValues(pixel_val, 255.0);
            // std::cout << image[i][j].at(0) << ", " << image[i][j].at(1) << ", " << image[i][j].at(2) << "\n";
            fg_pixel_count++;
            
        }
        
    }
    std::cout << "Outside the loop\n";
    

    // Create an empty image with 3 channels (BGR) and set it to a specific color
    cv::Mat imageMat(imgHeight, imgWidth, CV_32FC3); // Red color in BGR format


    // Fill the Mat with the values from the float list
    for (int i = 0; i < imgHeight; ++i) {
        for (int j = 0; j < imgWidth; ++j) {
            Vec3f intensity = imageMat.at<Vec3f>(Point(i,j));
            intensity.val[0] = image[i][j].at(2);
            intensity.val[1] = image[i][j].at(1);
            intensity.val[2] = image[i][j].at(0);
            imageMat.at<Vec3f>(Point(i,j)) = intensity;

            // (cv::Scalar(image[i][j].at(0),image[i][j].at(1),image[i][j].at(2)));
            // std::cout << image[i][j].at(0) << ", " << image[i][j].at(1) << ", " << image[i][j].at(2) << "\n";
        }
    }

    printf("# of fg pixels: %d\n", fg_pixel_count);
    cv::imshow("Image", imageMat);
    cv::waitKey(0); // Wait for a key press

    // Optionally, save the image to a file
    cv::imwrite("output_image.png", imageMat);


    


}