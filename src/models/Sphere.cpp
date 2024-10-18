#include "Sphere.h"
#include "../util/util.h"
#include <cmath>  // For sqrt function

Sphere::Sphere(float cx, float cy, float cz, float R, Material* material, ObjectType objectType)
{
    this->cx = cx;
    this->cy = cy;
    this->cz = cz;
    this->R = R;
    this->center = {cx, cy, cz};
    this->material = material;
    this->objectType = objectType;
}
Material* Sphere::getMaterial(){
    return this->material;
}
ObjectType Sphere::getObject(){
    return this->objectType;
}
float Sphere::Intersects(Ray ray){
    vector<float> d = ray.d;
    vector<float> o = ray.o;
    vector<float> c = this->center;
    // -d . ( o - c )
    float mindoc = dotProduct(d, vectorAdd(o, vectorScale(c, -1)));
    


    std::vector<float> ominc =  vectorAdd(o, vectorScale(c, -1));
    // std:cout << "Distance from ray center to sphere center: "<< getMagnitude(ominc) << "\n";
    float dd = dotProduct(d,d);
    float sqrt_el =  pow(mindoc, 2)   -    (dd * (getMagnitude(ominc) - pow(R, 2)));

    // float disc =  4 * pow(dotProduct(d, ominc), 2) - 4 * dd * (dotProduct(ominc, ominc) - pow(R,2));
    // means ray does not intersect with the spehere
    if (sqrt_el < 0){
        // std::cout << "Sphere does not intersect with the ray: " << sqrt_el << "\n" ;    
        return -1.0f;
    }
    float t1 = (dotProduct(vectorScale(d, -1), vectorAdd(o, vectorScale(c, -1))) + sqrt(sqrt_el)) / dd;
    float t2 = (dotProduct(vectorScale(d, -1), vectorAdd(o, vectorScale(c, -1))) - sqrt(sqrt_el)) / dd;


    // std::cout << "t1: " << t1 << "\n";
    // std::cout << "t2: " << t2 << "\n";
    
    // need the smaller one (positive)
    if (t1 > 0 && t1 < t2) {
        // std::cout << "Sphere hit by ray...\n";
        return t1;
    }
        
    if (t2 > 0 && t2 < t1)
        {
            // std::cout << "Sphere hit by ray...\n";
            return t2;
        }
    // std::cout << "Sphere is behind the ray\n";
    return -1.0;
}

vector<float> Sphere::getSurfaceNormal(vector<float> location){
    // surface normal can be calculated via getting normal vector that is in the direction from center to the location
    vector<float> scaledVector = vectorAdd(vectorScale(location, -1), this->center);
    vector<float> unitVector = vectorScale(scaledVector, 1/sqrt(dotProduct(scaledVector, scaledVector)));
    return unitVector;
}

