#include "src/models/Triangle.h"
#include "src/models/Camera.h"
#include "src/models/ImagePane.h"
#include "src/models/Ray.h"
#include "src/models/Sphere.h"
#include "src/lights/Light.h"
#include "src/lights/PointLight.h"
#include "src/scene/Scene.h"
#include "src/shaders/Shader.h"


#include <vector>
// for testing remove before submitting
#include <opencv2/opencv.hpp>


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
    Material* triangleMaterial = new Material(triangleMaterialType, ambientProp, diffuse, specular);
    float triangleVertices[3][3] = {{1,1,5}, {4,2,2}, {123,3,3}};
    Triangle* triangle = new Triangle(triangleMaterial, ObjectType::TriangleType, triangleVertices[0], triangleVertices[1], triangleVertices[2]);
    printf("Area of the triangle: %f\n", triangle->getArea());

    
    vector<float> u = {1,0,0};
    vector<float> v = {0,1,0};
    vector<float> e = {0,0,0};
    // intiialize camera
    Camera* c = new Camera(u, v, e);

    int imgHeight = 100;
    int imgWidth = 100;


    ImagePane* imagePane = new ImagePane(
        imgHeight, imgWidth, -imgHeight/2, imgWidth/2, -imgHeight/2, imgWidth/2, 10, c 
    );


    Sphere* sphere = new Sphere(
        -10, -10, -10, 5, triangleMaterial, ObjectType::SphereType
    );

    vector<float> intersectingDirection = {-1,-1,-1};
    Ray* intersectingRay = new Ray(
        o, intersectingDirection
    );
    float tval = sphere->Intersects(*intersectingRay);
    std::cout << "tvalue: " << tval << "\n";



    // now initialize scene and iteratively fill the ImagePane
    Object** objList = new Object*[1];
    std::cout << "I'll be setting spehere:..\n";
    objList[0] = sphere;
    std::cout << "Sphere set...\n";

    Light** lightList = new Light*[1];
    lightList[0] = new PointLight(5, {-6, -6, -6});

    std::cout << "Setting scene...\n";
    Scene scene = Scene(
        objList, 1, *c, *imagePane, lightList, 1
    );

    std::cout << "Scene initialized\n";

    Shader shader = Shader(
        &scene   
    );

    float image[imgWidth][imgHeight];

    // now we need to iterate over the pixels and fill them
    for (int i = 0; i < imgWidth; i++)
    {
        for (int j = 0; j < imgHeight; j++)
        {

            
            // shoot ray from camera to ImagePane
            Ray cameraRay = imagePane->rayFromCamera(i, j);
            
            // now iterate over the objects to find first object that hits this ray
            // add fov in future to exclude object that are too far away from the camera
            float minTValue = 9999999;
            int intersectingObjIndex = -1;
            
            for (int k = 0; k < 1; k++)
            {
                float tvalue = objList[k]->Intersects(cameraRay);
                if (tvalue > 0 && tvalue < minTValue){
                    minTValue = tvalue;
                    intersectingObjIndex = k;
                    std::cout << "For i,j:" << i <<  ", " << j  << " intersects with the object\n";
                }
            }

            if (intersectingObjIndex == -1){
                // means we need to set bg color for this pixel
                
                image[i][j] =255;
                continue;
            }

            
            
            std::cout << "Calculating the diffuse shading: \n";
            // if reached here then camera hits an object
            image[i][j] = shader.diffuseShadingAt(cameraRay.locationAtT(minTValue), objList[intersectingObjIndex]);
            std::cout << image[i][j] <<  "\n";
            
            
        }
        
    }
    std::cout << "Outside the loop\n";
    

    // Create an OpenCV Mat object to hold the image
    cv::Mat imageMat(imgWidth, imgHeight, CV_32F); // Use CV_32F for float representation

    // Fill the Mat with the values from the float list
    for (int i = 0; i < imgHeight; ++i) {
        for (int j = 0; j < imgWidth; ++j) {
            imageMat.at<float>(i, j) = image[i][j];
        }
    }

    
    // Normalize the values to the range [0, 255] for display
    cv::Mat imageNormalized;
    cv::normalize(imageMat, imageNormalized, 0, 255, cv::NORM_MINMAX, CV_8UC1); // Convert to 8-bit grayscale

    // Display the image
    cv::imshow("Grayscale Image", imageNormalized);
    cv::waitKey(0); // Wait for a key press

    // Optionally, save the image to a file
    cv::imwrite("output_image.png", imageNormalized);


    


}