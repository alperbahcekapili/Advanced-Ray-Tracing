#pragma once
#include "Light.h"
class PointLight : public Light
{
private:
    /* data */
public:
    PointLight(std::vector<float> intensity, std::vector<float> location);
    
    std::vector<float> irradianceAt(std::vector<float> location) override;
};
