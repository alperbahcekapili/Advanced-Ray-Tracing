#include "Scene.h"


Scene::Scene(Object** sceneObjects, int numObjects, Camera camera, ImagePane imagePane, Light** lights, int numlights): 
sceneObjects(sceneObjects), numObjects(numObjects), camera(camera), imagePane(imagePane), lights(lights), numlights(numlights)
{   
    
}

Scene::~Scene()
{
}
