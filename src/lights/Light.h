#pragma once
#include <vector>
#include "../util/data_structures.h"
#include "../models/Ray.h"


enum LightType{
    PointLightType,
    AreaLightType
};

class Light
{
private:
    
public:
    Vec3  location;
    Vec3  intensity;
    LightType ltype;
    virtual Vec3 getPointOn() = 0;
    virtual Vec3  irradianceAt(Ray light_ray, Vec3  location) = 0;
    virtual ~Light() = default;
};

