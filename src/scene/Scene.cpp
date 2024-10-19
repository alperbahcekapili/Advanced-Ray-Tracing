#include "Scene.h"


Scene::Scene(Object** sceneObjects, int numObjects, Camera camera, ImagePane imagePane, Light** lights, int numlights, std::vector<float> bg, std::vector<float> ambient_light): 
sceneObjects(sceneObjects), numObjects(numObjects), camera(camera), imagePane(imagePane), lights(lights), numlights(numlights), bg(bg), ambient_light(ambient_light)
{   
    
}

Scene::~Scene()
{
}
