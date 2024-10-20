#include "Shader.h"
#include "../models/Object.h"
#include "../models/Sphere.h"
#include "../util/util.h"
#include <math.h>

Shader::Shader(Scene* scene)
{
    this->scene = scene;

}

Shader::~Shader()
{
}

/*
This is the function that determines if light hist the object from the other side of the object or hits another object before hitting the one
*/
bool lightHits(Ray light_ray, std::vector<float> location, Object* intersectingObject, int intersectingObjIndex, Object** other_objects, int numobj ){
    // create a ray from the lightsource to destination
    
    // now we need to test if this ray intersects with any of the objects
    // if ray intersects with any objects other than intersecting object with smaller t value then this area
    // is not lighted by this lightsource
    
    float intersectingTvalue = intersectingObject->Intersects(light_ray);
    // std::cout << "Light hits the object in required area. with thvalue:"<< intersectingTvalue <<  "..\n"; 
    
    // we can calculate intersecting location with this tvalue and it is different 
    // from the given location then that means light hits the other side of the object
    vector<float> lightHitLocation = vectorAdd(light_ray.o, vectorScale(light_ray.d, intersectingTvalue));
    float errorMargin = 0.01;
    if(getMagnitude(vectorAdd(lightHitLocation, vectorScale(location, -1))) > errorMargin){
        // This means the light is in the other side of the object
        // std::cout << "Light is on the other side. for "<< intersectingObjIndex << " \n";
        return false;
    }
    bool in_shadow = false;
    for (int j = 0; j < numobj; j++)
    {
        if (j == intersectingObjIndex)
        continue;

        float tvalue = other_objects[j]->Intersects(light_ray);
        if( tvalue < intersectingTvalue &&  tvalue > 0){
            // std::cout << "intersecting: " << intersectingTvalue << ", interle. " << tvalue << "\n";
            // then this means light hits another object before hitting this location than we can retun 0 directly
            // std::cout << "Ligth is cut by another object, object camera sees: "<<  intersectingObjIndex <<" \n";
            in_shadow = true;
            break;
        }
        
    }
    if ( in_shadow){
        return false;;
    }
    return true;

       
}


vector<float> Shader::diffuseShadingAt(vector<float> location, Object* intersectingObject, int intersectingObjIndex){

    /*
    Iterate over the lightsources and detect if the location is lightened by the lightsource
    */

   vector<float> pixel = {0,0,0};

   for (int i = 0; i < this->scene->numlights; i++)
   {
    Ray lightRay = createRayFrom(this->scene->lights[i]->location, location);
    bool ligth_hits = lightHits(lightRay, location, intersectingObject, intersectingObjIndex, this->scene->sceneObjects, this->scene->numObjects)    ;;
    if (!ligth_hits)
        continue;

    // If we got so far then it means ith light source hits this surface thus we can calculate illumination
    float cosTheta = dotProduct(lightRay.d, intersectingObject->getSurfaceNormal(location));
    std::vector<float> irradiance = vectorScale(this->scene->lights[i]->irradianceAt(location), cosTheta);
    //std::cout << "Irradiance: " << irradiance.at(0) << "," << irradiance.at(1) << "," << irradiance.at(2) <<  "\n";
    //std::cout << "Cos: " << cosTheta <<  "\n";
    vector<float> tmp = vectorMultiplyElementwise(intersectingObject->getMaterial()->diffuseProp ,irradiance);
    pixel = vectorAdd(pixel, tmp);

   }
    return pixel;
}



vector<float> Shader::ambientShadingAt(vector<float> location, Object* intersectingObject, int intersectingObjIndex){
    return vectorMultiplyElementwise(intersectingObject->getMaterial()->ambientProp, this->scene->ambient_light);
}



vector<float> Shader::specularShadingAt(Ray cameraRay,vector<float> location, Object* intersectingObject, int intersectingObjIndex){
    std::vector<float> resultingMagnitude = {0,0,0};
    for (int i = 0; i < this->scene->numlights; i++)
    {
        // create a ray from the lightsource to destination
        Ray lightRay = createRayFrom(this->scene->lights[i]->location, location);
        bool ligth_hits = lightHits(lightRay, location, intersectingObject, intersectingObjIndex, this->scene->sceneObjects, this->scene->numObjects)    ;;
        if (!ligth_hits)
            continue;

        std::vector<float> surface_normal = intersectingObject->getSurfaceNormal(location);
        std::vector<float> tmp = vectorAdd(cameraRay.d, lightRay.d);
        float costheta  = dotProduct(surface_normal,tmp) / (getMagnitude(surface_normal) * getMagnitude(tmp));
        if (costheta < 0){
            costheta = 0;
        }else{
            costheta = pow(costheta, intersectingObject->getMaterial()->phong_exponent);
        }
        resultingMagnitude = vectorAdd(resultingMagnitude,vectorMultiplyElementwise(intersectingObject->getMaterial()->specularProp, vectorScale(this->scene->lights[i]->irradianceAt(location),  costheta)));
    }

    return resultingMagnitude;

}


