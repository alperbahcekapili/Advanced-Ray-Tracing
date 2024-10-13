#include <vector>
#pragma once




class Light
{
private:
    
public:
    std::vector<float> location;
    float intensity;
    virtual float irradianceAt(std::vector<float> location) = 0;
};

