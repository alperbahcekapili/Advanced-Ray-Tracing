#include "PointLight.h"
#include "Light.h"
#include <vector>
#include "../util/util.h"
#include <math.h>

PointLight::PointLight(std::vector<float> intensity, std::vector<float> location)
{
    this->intensity = intensity;
    this->location = location;
}


std::vector<float> PointLight::irradianceAt(std::vector<float> location)  {
    float distance = getMagnitude(vectorAdd(this->location, vectorScale(location, -1)));
    return vectorScale(this->intensity, 1/distance);
    
}


