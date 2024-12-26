#include "SpotLight.h"

SpotLight::SpotLight(Vec3 position, Vec3 direction, Vec3 intensity, float coverage_angle, float falloff_angle){
    this->position = position;
    this->direction = direction.normalize();
    this->intensity = intensity;
    this->coverage_angle = coverage_angle * M_PI / 180 ;
    this->falloff_angle = falloff_angle * M_PI / 180;
}


Vec3  SpotLight::irradianceAt(Ray light_ray, Vec3  location)  {
    // first calculate the angle
    float d = (location - this->position ).magnitude();
    float dot = std::clamp(this->direction.dot(light_ray.d), -1.0f, 1.0f);
    float alpha = acos(dot);
    if(alpha < this->falloff_angle / 2){
        return this->intensity / pow(d , 2);
    }else if (alpha < this->coverage_angle / 2)
    {
        float s = pow((cos(alpha) - cos(coverage_angle/2)) / (cos(falloff_angle/2) - cos(coverage_angle/2)), 4);
        return (s * this->intensity) / pow(d , 2);
    }
    return 0;
    
}

Vec3 SpotLight::getPointOn(){
    return this->position;
}