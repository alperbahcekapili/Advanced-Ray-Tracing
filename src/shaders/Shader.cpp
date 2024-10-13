#include "Shader.h"
#include "../models/Object.h"
#include "../models/Sphere.h"
#include "../models/util/util.h"


Shader::Shader(Scene* scene)
{
    this->scene = scene;

}

Shader::~Shader()
{
}

float Shader::diffuseShadingAt(vector<float> location, Object* intersectingObject){

    /*
    Iterate over the lightsources and detect if the location is lightened by the lightsource
    */


   float resultingMagnitude = 0;
   for (int i = 0; i < this->scene->numlights; i++)
   {
    // create a ray from the lightsource to destination
    Ray lightRay = createRayFrom(this->scene->lights[i]->location, location);
    // now we need to test if this ray intersects with any of the objects
    // if ray intersects with any objects other than intersecting object with smaller t value then this area
    // is not lighted by this lightsource
    
    float intersectingTvalue = intersectingObject->Intersects(lightRay);
    // we can calculate intersecting location with this tvalue and it is different 
    // from the given location then that means light hits the other side of the object
    vector<float> lightHitLocation = vectorAdd(lightRay.o, vectorScale(lightRay.d, intersectingTvalue));
    float errorMargin = 0.01;
    if(dotProduct(vectorAdd(lightHitLocation, vectorScale(location, -1)), vectorAdd(lightHitLocation, vectorScale(location, -1))) > errorMargin){
        // this means light is cut befor hitting this location
        return 0;
    }

    for (int j = 0; j < this->scene->numObjects; j++)
    {
        float tvalue = this->scene->sceneObjects[j]->Intersects(lightRay);
        if( tvalue < intersectingTvalue ){
            // then this means light hits another object before hitting this location than we can retun 0 directly
            return 0;
        }
    }

    // If we got so far then it means ith light source hits this surface thus we can calculate illumination
    float cosTheta = dotProduct(lightRay.d, intersectingObject->getSurfaceNormal(location)); // because each of these vectors are unit we can directly calculate dot product
    resultingMagnitude += intersectingObject->material->diffuseProp * this->scene->lights[i]->irradianceAt(location) * cosTheta;
   }
   

    return resultingMagnitude;
}