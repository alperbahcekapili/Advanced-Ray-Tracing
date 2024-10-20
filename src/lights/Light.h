#include <vector>
#pragma once




class Light
{
private:
    
public:
    std::vector<float> location;
    std::vector<float> intensity;
    virtual std::vector<float> irradianceAt(std::vector<float> location) = 0;
};

