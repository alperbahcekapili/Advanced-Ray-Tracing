#pragma once
#include "Light.h"

class AreaLight : public Light{
    private:
    public:
    float width;
    float area;
    Vec3 position;
    Vec3 normal;
    Vec3 radiance;
    AreaLight(float width, Vec3 position, Vec3 normal, Vec3 radiance);
    Vec3 getPointOn() override;
    Vec3 irradianceAt(Ray light_ray, Vec3  location) override;
};

