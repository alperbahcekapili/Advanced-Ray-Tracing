#pragma once

#include <vector>
#include <string>
class Camera
{
private:
    // in camera space
    std::vector<float> position; // e
    
public:
    Camera(std::vector<float> v, std::vector<float> w, std::vector<float> position, float maxt, float mint, std::string name);
    std::vector<float> getPosition();
    std::vector<float> w; // I use w as the gaze direction
    std::vector<float> u;
    std::vector<float> v;
    std::string name;
    float mint;
    float maxt;
};

