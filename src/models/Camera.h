#pragma once

#include <vector>
class Camera
{
private:
    // in camera space
    std::vector<float> position; // e
    
public:
    Camera(std::vector<float> u, std::vector<float> v, std::vector<float> position);
    std::vector<float> getPosition();
    std::vector<float> w;
    std::vector<float> u;
    std::vector<float> v;
};

