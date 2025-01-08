#pragma once
#include <vector>
#include "../util/data_structures.h"
#include "../models/Ray.h"


enum LightType{
    PointLightType,
    AreaLightType,
    DirectionalLightType,
    SphericalDirectionalLightType
};

class Light
{
private:
    
public:
    Vec3  location;
    Vec3  intensity;
    LightType ltype;
    virtual Vec3 getPointOn() {};
    virtual Vec3  irradianceAt(Ray light_ray, Vec3  location) {};
    virtual ~Light() = default;
};

