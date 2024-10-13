#pragma once
#include "Light.h"
class PointLight : public Light
{
private:
    /* data */
public:
    PointLight(float intensity, std::vector<float> location);
    
    float irradianceAt(std::vector<float> location) override;
};
