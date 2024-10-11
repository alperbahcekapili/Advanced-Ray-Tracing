#include "Sphere.h"
#include "util/util.h"
#include <cmath>  // For sqrt function

Sphere::Sphere(float cx, float cy, float cz, float R, Material* material, ObjectType objectType):
Object(material, objectType)
{
    this->cx = cx;
    this->cy = cy;
    this->cz = cz;
    this->R = R;
}

float Sphere::Intersects(Ray ray){
    vector<float> d = ray.d;
    vector<float> o = ray.o;
    vector<float> c = {this->cx, this->cy, this->cz};
    // -d . ( o - c )
    float mindoc = dotProduct(vectorScale(d, -1), vectorAdd(o, vectorScale(c, -1)));
    float dd = dotProduct(d,d);
    float disc = (4 * dotProduct(d, vectorAdd(o, vectorScale(c, -1))) * dotProduct(d, vectorAdd(o, vectorScale(c, -1)))) - (dd * dotProduct(vectorAdd(o, vectorScale(c, -1)),vectorAdd(o, vectorScale(c, -1)))) - R*R;

    // means ray does not intersect with the spehere
    if (disc < 0){
        std::cout << "Sphere does not intersect with the ray";    
        return -1.0f;
    }
    float t1 = (mindoc + sqrt(disc)) / dd;
    float t2 = (mindoc - sqrt(disc)) / dd;
    
    // need the smaller one (positive)
    if (t1 > 0 && t1 < t2)
        return t1;
    if (t2 > 0 && t2 < t1)
        return t2;
    std::cout << "t1: " << t1 << "\n";
    std::cout << "t2: " << t2 << "\n";
    std::cout << "Sphere is behind the ray\n";
    return -1.0;
}

Sphere::~Sphere()
{
}
