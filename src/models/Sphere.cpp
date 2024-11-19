#include "Sphere.h"
#include "../util/util.h"
#include <cmath>  // For sqrt function


TransformationMatrix* Sphere::gettm(){
    return this->tm;
}


Sphere::Sphere(Vec3 center, float R, Material* material, ObjectType objectType, TransformationMatrix* tm)
{
    this->R = R;
    this->center = center;
    this->material = material;
    this->objectType = objectType;
    this->tm = new TransformationMatrix();
    Vec3 point_on_sphere = Vec3(center.x + R, center.y, center.z);
    TransformationMatrix* to_center = new TransformationMatrix(-1*center, 't');
    TransformationMatrix* from_center = new TransformationMatrix(center, 't');

    // We apply the same transformation to a point on the sphere then calculate new R
    

    *(this->tm) = (*from_center) * (*tm) * (*to_center);
    
    center = this->tm->transform(this->center);
    this->center = center;
    
    Vec3 new_pointon = this->tm->transform(point_on_sphere);
    R = (new_pointon-center).magnitude();
    this->R = R;

    this->min = Vec3(center.x-R, center.y-R, center.z-R);
    this->max = Vec3(center.x+R, center.y+R, center.z+R);

}

Vec3 Sphere::getBoundingBox(bool isMax){
    if(isMax)
        return this->max;
    return this->min;
}
Material* Sphere::getMaterial(){
    return this->material;
}
ObjectType Sphere::getObject(){
    return this->objectType;
}

Vec3  Sphere::getCenter(){
    return this->center;
}


float Sphere::Intersects(Ray ray){
    Vec3 d = ray.d;
    Vec3 o = ray.o;
    Vec3 c = this->center;
    // -d . ( o - c )
    Vec3 ominc =  o - c; 
    float mindoc = -1 * d.dot(ominc);

    // std:cout << "Distance from ray center to sphere center: "<< getMagnitude(ominc) << "\n";

    float dd = d.dot(d);
    float sqrt_el =  pow(mindoc, 2)   -    (dd * (ominc.dot(ominc) - pow(R, 2)));

    // float disc =  4 * pow(dotProduct(d, ominc), 2) - 4 * dd * (dotProduct(ominc, ominc) - pow(R,2));
    // means ray does not intersect with the spehere
    if (sqrt_el < 0){
        // std::cout << "Sphere does not intersect with the ray: " << sqrt_el << "\n" ;    
        return -1.0f;
    }
    float t1 = (((d * -1).dot(o + (c * -1))) + sqrt(sqrt_el)) / dd;
    float t2 = (((d * -1).dot(o + (c * -1))) - sqrt(sqrt_el)) / dd;

    
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

    if (t1>0 && t2 < 0)
        return t1;
    if (t2> 0 && t1 < 0)
        return t2;
    // std::cout << "Sphere is behind the ray\n";
    
    return -1.0;
}

Vec3 Sphere::getSurfaceNormal(Ray r){
    float mint = this->Intersects(r);
    if(mint < 0)
        return Vec3(-1,-1,-1);
    Vec3 location = r.locationAtT(mint);
    Vec3 scaledVector = (this->center * -1) + location ;
    Vec3 unitVector = scaledVector.normalize();
    return unitVector;
}

