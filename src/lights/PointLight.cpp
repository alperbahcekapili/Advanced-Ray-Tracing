#include "PointLight.h"
#include "Light.h"
#include <vector>
#include "../models/util/util.h"
#include <math.h>

PointLight::PointLight(float intensity, std::vector<float> location)
{
    this->intensity = intensity;
    this->location = location;
}


float PointLight::irradianceAt(std::vector<float> location)  {
    return this->intensity / pow(getMagnitude(vectorAdd(this->location, vectorScale(location, -1))),2);
}


