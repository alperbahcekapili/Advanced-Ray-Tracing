#pragma once
#include "Light.h"
class PointLight : public Light
{
private:
    /* data */
public:
    PointLight(Vec3  intensity, Vec3  location);
    
    Vec3  irradianceAt(Vec3  location) override;
};
