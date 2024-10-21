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
    Camera* camera;
    int numObjects;  
    Object** sceneObjects;
       
    ImagePane* imagePane; 
    Light** lights; 
    int numlights;
    std::vector<int> ambient_light;
    std::vector<int> bg;
    float refraction_index;
    float shadow_ray_eps;
    
    Scene(Object** sceneObjects, int numObects, Camera* camera, ImagePane* imagePane, Light** lights, int numlights, int bg[3], int ambient_light[3], float shadow_ray_eps, float refraction_index);
    ~Scene();
};
