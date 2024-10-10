#include "src/models/Triangle.h"
#include <vector>


int main(){
    vector<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    vector<int> o;
    o.push_back(0);
    o.push_back(0);
    o.push_back(0);

    int t = 5;
    Ray* ray = new Ray(d = d, o = o, t = t);
    cout << ray->toString();


    MaterialType triangleMaterialType = MaterialType::Mirror;
    int ambientProp[3] = {4,5,1};
    int diffuse[3] = {5,6,1};
    int specular[3] = {6,7,1};
    Material* triangleMaterial = new Material(triangleMaterialType, ambientProp, diffuse, specular);
    double triangleVertices[3][3] = {{1,1,5}, {4,2,2}, {123,3,3}};
    Triangle* triangle = new Triangle(triangleMaterial, ObjectType::TriangleType, triangleVertices[0], triangleVertices[1], triangleVertices[2]);
    printf("Area of the triangle: %f\n", triangle->getArea());

    



}