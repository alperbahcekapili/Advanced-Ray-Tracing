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
    int numlights;
    Scene(Object** sceneObjects, int numObjects, Camera camera, ImagePane imagePane, Light** lights, int numlights);
    ~Scene();
};
