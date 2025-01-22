#include "Scene.h"
#include "../util/data_structures.h"
Object* Scene::getObjectWithID(int id){
    return this->bvh->searchID(id);
}
Scene::Scene(Object** sceneObjects, int numObjects, Camera* camera, ImagePane* imagePane, Light** lights, int numlights, Vec3 bg, Vec3 ambient_light, float shadow_ray_eps, float refraction_index){   
    this->shadow_ray_eps= shadow_ray_eps;
    this->numObjects = numObjects;
    this->numlights = numlights;
    this->sceneObjects = sceneObjects;
    this->camera = camera;
    this->imagePane = imagePane;
    this->lights  = lights;
    this->bg = bg;
    this->ambient_light = ambient_light;
    this->refraction_index = refraction_index;
}

Scene::~Scene()
{
}
