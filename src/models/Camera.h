#pragma once

#include <vector>
#include <string>
#include "../util/data_structures.h"
class Camera
{
private:
    // in camera space
    Vec3  position; // e
    
public:
    Camera(Vec3  v, Vec3  w, Vec3  position, float maxt, float mint, std::string name, float numsamples, float focus_distance, float aperture_size);
    Vec3  getPosition();
    Vec3  w; // I use w as the gaze direction
    Vec3  u;
    Vec3  v;
    std::string name;
    float mint;
    float maxt;
    float numsamples;
    float focus_distance;
    float aperture_size;
    float gamma;
    float keyvalue;
    float burnout;
    float saturation;
};

