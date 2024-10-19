#pragma once 

#include "../models/Object.h"
#include "../models/Camera.h"
#include "../models/ImagePane.h"
#include "../lights/Light.h"

class Scene
{
private:
    /* data */
public:
    Object** sceneObjects;
    int numObjects;
    Camera camera; 
    ImagePane imagePane; 
    Light** lights; 
    std::vector<float> ambient_light;
    int numlights;
    std::vector<float> bg;
    Scene(Object** sceneObjects, int numObjects, Camera camera, ImagePane imagePane, Light** lights, int numlights, std::vector<float> bg, std::vector<float> ambient_light);
    ~Scene();
};
