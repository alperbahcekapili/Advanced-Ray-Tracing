#include "src/models/Triangle.h"
#include "src/models/Camera.h"
#include "src/models/ImagePane.h"
#include "src/models/Ray.h"
#include "src/models/Sphere.h"
#include <vector>


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
    float diffuse[3] = {5,6,1};
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


    ImagePane* imagePane = new ImagePane(
        10, 10, -5, 5, -5, 5, 8, c 
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



}