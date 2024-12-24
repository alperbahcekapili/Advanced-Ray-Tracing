#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(Vec3 direction, Vec3 radiance){
    this->direction = direction;
    this->radiance = radiance;
    this->ltype = DirectionalLightType;
}

Vec3 DirectionalLight::irradianceAt(Ray light_ray, Vec3  location){
    return this->radiance;
}

Vec3 DirectionalLight::getPointOn(){
    return Vec3(0,0,0);
}

Vec3 DirectionalLight::getDirection(){
    return this->direction;
}
Vec3 DirectionalLight::getRadiance(){
    return this->radiance;
}