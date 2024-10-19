#include "Shader.h"
#include "../models/Object.h"
#include "../models/Sphere.h"
#include "../util/util.h"


Shader::Shader(Scene* scene)
{
    this->scene = scene;

}

Shader::~Shader()
{
}

vector<float> Shader::diffuseShadingAt(vector<float> location, Object* intersectingObject, int intersectingObjIndex){

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
    // std::cout << "Light hits the object in required area. with thvalue:"<< intersectingTvalue <<  "..\n"; 
    
    // we can calculate intersecting location with this tvalue and it is different 
    // from the given location then that means light hits the other side of the object
    vector<float> lightHitLocation = vectorAdd(lightRay.o, vectorScale(lightRay.d, intersectingTvalue));
    float errorMargin = 0.01;
    if(getMagnitude(vectorAdd(lightHitLocation, vectorScale(location, -1))) > errorMargin){
        // This means the light is in the other side of the object
        // std::cout << "Light is on the other side. for "<< intersectingObjIndex << " \n";
        continue;;
    }
    bool in_shadow = false;
    for (int j = 0; j < this->scene->numObjects; j++)
    {
        if (j == intersectingObjIndex)
        continue;

        float tvalue = this->scene->sceneObjects[j]->Intersects(lightRay);
        if( tvalue < intersectingTvalue &&  tvalue > 0){
            // std::cout << "intersecting: " << intersectingTvalue << ", interle. " << tvalue << "\n";
            // then this means light hits another object before hitting this location than we can retun 0 directly
            // std::cout << "Ligth is cut by another object, object camera sees: "<<  intersectingObjIndex <<" \n";
            in_shadow = true;
            break;
        }
        
    }
    if ( in_shadow){
        continue;;
    }
        

    // If we got so far then it means ith light source hits this surface thus we can calculate illumination
    float cosTheta = dotProduct(lightRay.d, intersectingObject->getSurfaceNormal(location));
    float irradiance = this->scene->lights[i]->irradianceAt(location) * cosTheta;
    // std::cout << "Irradiance: " << irradiance <<  "\n";
    // std::cout << "Cos: " << cosTheta <<  "\n";
    resultingMagnitude += intersectingObject->getMaterial()->diffuseProp * irradiance;
   }
   

    vector<float> retval = vectorScale(intersectingObject->getMaterial()->color ,resultingMagnitude);
    return retval;
}



vector<float> Shader::ambientShadingAt(vector<float> location, Object* intersectingObject, int intersectingObjIndex){
    return vectorMultiplyElementwise(intersectingObject->getMaterial()->ambientProp, this->scene->ambient_light);
}



vector<float> Shader::specularShadingAt(Ray cameraRay,vector<float> location, Object* intersectingObject, int intersectingObjIndex){
    std::vector<float> resultingMagnitude = {0,0,0};
    bool no_light = true;
    for (int i = 0; i < this->scene->numlights; i++)
    {
        // create a ray from the lightsource to destination
        Ray lightRay = createRayFrom(this->scene->lights[i]->location, location);
        // now we need to test if this ray intersects with any of the objects
        // if ray intersects with any objects other than intersecting object with smaller t value then this area
        // is not lighted by this lightsource
        
        float intersectingTvalue = intersectingObject->Intersects(lightRay);
        // std::cout << "Light hits the object in required area. with thvalue:"<< intersectingTvalue <<  "..\n"; 
        
        // we can calculate intersecting location with this tvalue and it is different 
        // from the given location then that means light hits the other side of the object
        vector<float> lightHitLocation = vectorAdd(lightRay.o, vectorScale(lightRay.d, intersectingTvalue));
        float errorMargin = 0.1;
        if(getMagnitude(vectorAdd(lightHitLocation, vectorScale(location, -1))) > errorMargin){
            // this means light is cut befor hitting this location
            // Shadow ray !
            // std::cout << "Light is cut before \n";
            continue;;
        }

        bool in_shadow = false;
        for (int j = 0; j < this->scene->numObjects; j++)
        {
            if (j == intersectingObjIndex)
            continue;
            float tvalue = this->scene->sceneObjects[j]->Intersects(lightRay);
            if( tvalue < intersectingTvalue && tvalue > 0){
                // std::cout << "intersecting: " << intersectingTvalue << ", interle. " << tvalue << "\n";
                // then this means light hits another object before hitting this location than we can retun 0 directly
                // std::cout << "Ligth is cut by another object, object camera sees: "<<  intersectingObjIndex <<" \n";
                in_shadow = true;
                break;
            }
        }
        if ( in_shadow){
            continue;;
        }else{
            no_light = false;
        }

        if(no_light)
            return {0,0,0};

        // here we know light hits the surface directly
        std::vector<float> surface_normal = intersectingObject->getSurfaceNormal(location);
        std::vector<float> tmp = vectorAdd(cameraRay.d, lightRay.d);
        float costheta  = dotProduct(surface_normal,tmp) / (getMagnitude(surface_normal) * getMagnitude(tmp));
        if (costheta < 0){
            std::cout << "I calculate negative costheta in specular shading";
            costheta = 0;
        }else{
            costheta = pow(costheta, intersectingObject->getMaterial()->phong_exponent);
        }
        resultingMagnitude = vectorAdd(resultingMagnitude,vectorScale(intersectingObject->getMaterial()->specularProp, costheta * this->scene->lights[i]->irradianceAt(location)));
    }

    return resultingMagnitude;

}


