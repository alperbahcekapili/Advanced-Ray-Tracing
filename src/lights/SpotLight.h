
#pragma once
#include "Light.h"

class SpotLight : public Light
{
    private:
    public: 
        Vec3 position;
        Vec3 direction;
        Vec3 intensity;
        float coverage_angle;
        float falloff_angle;
        
        SpotLight(Vec3 position, Vec3 direction, Vec3 intensity, float coverage_angle, float falloff_angle);


        Vec3 irradianceAt(Ray light_ray, Vec3  location) override;
        Vec3 getPointOn() override;

};