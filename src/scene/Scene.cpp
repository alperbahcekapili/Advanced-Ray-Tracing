#include "Scene.h"


Scene::Scene(Object** sceneObjects, int numObjects, Camera camera, ImagePane imagePane, Light** lights, int numlights, std::vector<float> bg): 
sceneObjects(sceneObjects), numObjects(numObjects), camera(camera), imagePane(imagePane), lights(lights), numlights(numlights), bg(bg)
{   
    
}

Scene::~Scene()
{
}
