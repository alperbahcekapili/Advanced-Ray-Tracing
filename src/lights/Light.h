#pragma once
#include <vector>
#include "../util/data_structures.h"




class Light
{
private:
    
public:
    Vec3  location;
    Vec3  intensity;
    virtual Vec3  irradianceAt(Vec3  location) = 0;

    virtual ~Light() = default;
};

