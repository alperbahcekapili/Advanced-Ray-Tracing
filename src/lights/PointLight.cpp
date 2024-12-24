#include "PointLight.h"
#include "Light.h"
#include <vector>
#include "../util/util.h"
#include <math.h>

PointLight::PointLight(Vec3  intensity, Vec3  location)
{
    this->intensity = intensity;
    this->location = location;
}


Vec3  PointLight::irradianceAt(Ray light_ray, Vec3  location)  {
    float distance = (this->location - location).magnitude();
    return this->intensity * pow(1/distance, 2);    
}

Vec3 PointLight::getPointOn(){
    return this->location;
}
