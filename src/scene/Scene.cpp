#include "Scene.h"


Scene::Scene(Object** sceneObjects, int numObjects, Camera* camera, ImagePane* imagePane, Light** lights, int numlights, int bg[3], int ambient_light[3], float shadow_ray_eps, float refraction_index){   
    this->shadow_ray_eps= shadow_ray_eps;
    this->numObjects = numObjects;
    this->numlights = numlights;
    this->sceneObjects = sceneObjects;
    this->camera = camera;
    this->imagePane = imagePane;
    this->lights  = lights;
    std::cout << "My num obj: " << numObjects << "\n";
    this->bg = {bg[0],bg[1],bg[2]};
    this->ambient_light = {ambient_light[0],ambient_light[1],ambient_light[2]};
    this->refraction_index = refraction_index;
}

Scene::~Scene()
{
}
