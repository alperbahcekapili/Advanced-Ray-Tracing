#pragma once 

#include "../models/Object.h"
#include "../models/Camera.h"
#include "../models/ImagePane.h"
#include "../lights/Light.h"
#include "../acceleration/BVH.h"
class Scene
{
private:
    /* data */
public:
    Camera* camera;
    int numObjects;  
    int max_recursion_depth=1;
    int min_recursion_depth=1;
    Object** sceneObjects;
    ImagePane* imagePane; 
    Light** lights; 
    int numlights;
    Vec3 ambient_light;
    Vec3 bg;
    float refraction_index;
    float shadow_ray_eps;
    std::vector<Vec3> corner_grads;
    BVH* bvh;
    TextureMap* bg_texture;
    bool bg_texture_set;    
    bool spherical_light_flag; // means the background will be created by this light
    Light* spherical_light;
    Scene(Object** sceneObjects, int numObects, Camera* camera, ImagePane* imagePane, Light** lights, int numlights, Vec3 bg, Vec3 ambient_light, float shadow_ray_eps, float refraction_index);
    ~Scene();
};
