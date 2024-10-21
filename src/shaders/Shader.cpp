#include "Shader.h"
#include "../models/Object.h"
#include "../models/Sphere.h"
#include "../models/Material.h"
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

std::vector<float> Shader::specularReflection(Ray r, Scene* scene, Object* target_obj, int remaining_hop, int intersect_index){
    
    Assumes intersects with the target_obj
    Sends recursive rays in order to make reflection. 
    
    1. If target is not a mirror then it does not reflect incoming ray it returns its color directly
    2. If target is a mirror then it reflects the ray:
    3. First we need to calculate new redirected ray



   // color of target
   std::vector<float> intersecting_poisition = r.locationAtT(target_obj->Intersects(r));
   vector<float> diffuse_intensity = this->diffuseShadingAt(intersecting_poisition, target_obj, intersect_index);
    vector<float> ambient_intensity = this->ambientShadingAt(intersecting_poisition, target_obj, intersect_index);
    vector<float> specular_intensity = this->specularShadingAt(r, intersecting_poisition ,scene->sceneObjects[intersect_index], intersect_index);
    vector<float> pixel_val = vectorAdd(specular_intensity, vectorAdd(diffuse_intensity, ambient_intensity));


    if (remaining_hop == 0 || target_obj->getMaterial()->materialType != MaterialType::Mirror){
        
        
        // TODO: change hardcoded val
        return vectorScale(pixel_val,  0.01);
    }

    // wo
    std::vector<float> n = scene->sceneObjects[intersect_index]->getSurfaceNormal(intersecting_poisition);
    std::vector<float> wo = vectorScale(r.d, -1);
    float cos_theta = dotProduct(n, wo);
    
    Ray* wr = new Ray(intersecting_poisition,  vectorAdd(vectorScale(wo, -1), vectorScale(n, 2 * cos_theta)));

    // move the shadow ray along its direction to prevent self intersection
    wr->o = vectorAdd(wr->o, vectorScale(wr->d, scene->shadow_ray_eps));
    // new ray is wr
    
    float mint_value = 999999;
    int new_intersect_index = -1;
    for (int k = 0; k < scene->numObjects; k++)
    {
        float tvalue = scene->sceneObjects[k]->Intersects(*wr);
        if (tvalue > 0 && tvalue < mint_value){
            mint_value = tvalue;
            new_intersect_index = k;
        }
    }

    if (new_intersect_index == -1){
        std::cout << "Type is mirror but reflected ray does not intersect with r " <<  remaining_hop <<  "\n" ;
        // TODO: change hardcoded val
        return vectorScale(pixel_val,  0.01);
    }
        



    return vectorAdd(vectorScale(specularReflection(*wr, scene, scene->sceneObjects[new_intersect_index], remaining_hop -1, intersect_index), 0.1), vectorMultiplyElementwise(scene->sceneObjects[intersect_index ]->getMaterial()->mirrorReflectance , pixel_val));
    
    
*/

std::vector<float> Shader::radianceAt(std::vector<float> location, Object* intersectingObject, int intersectingObjIndex){
    std::vector<float> radiance = {0,0,0};
    for (int i = 0; i < this->scene->numlights; i++)
        {
            Ray lightRay = createRayFrom(this->scene->lights[i]->location, location);
            bool ligth_hits = lightHits(lightRay, location, intersectingObject, intersectingObjIndex, this->scene->sceneObjects, this->scene->numObjects)    ;;
            if (!ligth_hits)
                continue;

            // If we got so far then it means ith light source hits this surface thus we can calculate illumination
            float cosTheta = dotProduct(lightRay.d, intersectingObject->getSurfaceNormal(location));
            std::vector<float> irradiance = vectorScale(this->scene->lights[i]->irradianceAt(location), cosTheta);
            radiance = vectorAdd(radiance, irradiance);
        }

    return radiance;
}   


/*
This function calculates refraction and transmission
*/
std::vector<float> Shader::refractionTransmission(Ray r, Scene* scene, Object* target_obj, int remaining_hop, int intersect_index, bool inside){

    std::cout << "Remaninig hop: " << remaining_hop << "\n";
    if(target_obj->getMaterial()->materialType == MaterialType::Mirror)
        // We do not apply fresnel fraction to mirrors
        return {0,0,0};

    if(remaining_hop == -1)
        return {0,0,0};
    

    std::vector<float> intersecting_location = r.locationAtT(target_obj->Intersects(r));
    std::cout << "Intersecting location: " << intersecting_location.at(0) << ", " << intersecting_location.at(1) << ", "  << intersecting_location.at(2) << "\n" ;
    std::vector<float> n = target_obj->getSurfaceNormal(intersecting_location);
    if (inside)
        n = vectorScale(n,-1);
    // std::cout << "n: " << n.at(0) << ", "<< n.at(1) << ", " << n.at(2) << "\n";
    float cos_theta = dotProduct(vectorScale(r.d, -1), n);
    float n1 = scene->refraction_index;
    float n2 = target_obj->getMaterial()->refraction_index;

    // std::cout << "n1,n2: " << n1 << ", " << n2 << "\n";
    if (inside){
        float tmp = n1;
        n1 = n2;
        n2 = tmp;
    }
    std::cout << "n1,n2: " << n1 << ", " << n2 << "\n";
    std::cout << "ray direction: " << r.d.at(0) << ", " << r.d.at(1) << ", " << r.d.at(2) << "\n";
    std::cout << "cos theta: " << cos_theta << "\n";
    float before_sqrt = 1-(pow(n1/n2, 2)*(1-pow(cos_theta,2)));
    Ray reflected_ray = Ray(intersecting_location, vectorAdd(r.d , vectorScale(n, 2 * cos_theta)));
    // TODO replace this with shadow eps
    reflected_ray.o = vectorAdd(reflected_ray.o, vectorScale(reflected_ray.d, 0.01));
    // We need to move reflected ray along its direction in order to prevent self intersection



    // for dielectric objects this cannot be negative ?
    if (target_obj->getMaterial()->materialType == MaterialType::Dielectric){
        float cosphi = sqrt(before_sqrt);
        std::cout << "Before sqrt: "<< before_sqrt <<" Cosphi: " << cosphi << "\n";
        Ray transmitted_ray = Ray(intersecting_location, vectorSubstract(  vectorScale(vectorAdd(r.d , vectorScale(n, cos_theta)), n1/n2), vectorScale(n, cosphi) ) );
        // TODO replace below with shadow eps        
        transmitted_ray.o = vectorAdd(transmitted_ray.o, vectorScale(transmitted_ray.d, 1));


        float rpar = (n2 * cos_theta - n1 * cos_theta) / (n2 * cos_theta + n1 * cos_theta);
        float rper = (n1 * cos_theta - n2 * cosphi) / (n1 * cos_theta - n2 * cosphi);
        float fr = (pow(rpar,2) + pow(rper,2)) / 2;
        float ft = 1 - fr;
        std::cout << "For dielectric obj: fr: " <<  fr << ", ft: " << ft <<  "\n";
        std::vector<float> lx = this->radianceAt(intersecting_location, target_obj, intersect_index);
        
        if (inside){
            // attenuate
            // ray starts from r.o and inside the object
            std::vector<float> l0 = this->radianceAt(r.o, target_obj, intersect_index);
            float x = sqrt(getMagnitude(vectorSubstract(intersecting_location, r.o)));
            lx = vectorScale( l0 , exp(-1*target_obj->getMaterial()->absorption_coef*x));
            
        }

        
        std::vector<float> reflected_return = {0,0,0};
        std::vector<float> transmitted_return = {0,0,0};
        // we start 2 calls for dielectric one for transmission one for refraction
        // we can optimize here TODO:
        // now we calculated current effect we need to determine if the next ray hits another object. If so we recurse if not we terminate here
        float minTValue = 9999999;
        int next_intersecting_index = -1;


        for (int k = 0; k < this->scene->numObjects; k++)
        {
            float tvalue = this->scene->sceneObjects[k]->Intersects(transmitted_ray);
            if (tvalue > 0 && tvalue < minTValue){
                minTValue = tvalue;
                next_intersecting_index = k;
            }
        }
        std::cout << "Next objects index is : " << next_intersecting_index << "\n";
        std::cout << "transmitted ray: o: " << transmitted_ray.o.at(0) << ", "  << transmitted_ray.o.at(1) << ", "  << transmitted_ray.o.at(2) << ",  d:  "  << transmitted_ray.d.at(0) << ", "  << transmitted_ray.d.at(1) << ", "  << transmitted_ray.d.at(2) << "\n";

        if (next_intersecting_index != -1){
            std::cout << "I transmit a ray trough myself, inside: " << inside << ", maxhops remaining: " << remaining_hop << "\n";
            
            transmitted_return = vectorScale(
                vectorMultiplyElementwise(refractionTransmission(transmitted_ray, scene, this->scene->sceneObjects[next_intersecting_index], remaining_hop-1, next_intersecting_index, !inside), 
            target_obj->getMaterial()->mirrorReflectance),ft);
            
        }else{
            std::cout << "I cannot transmit a ray trough myself, inside: " << inside << ", maxhops remaining: " << remaining_hop << "\n";
        }



        
        for (int k = 0; k < this->scene->numObjects; k++)
        {
            float tvalue = this->scene->sceneObjects[k]->Intersects(reflected_ray);
            if (tvalue > 0 && tvalue < minTValue){
                minTValue = tvalue;
                next_intersecting_index = k;
            }
        }
        
        if (next_intersecting_index != -1){
            reflected_return = vectorScale(vectorMultiplyElementwise(refractionTransmission(reflected_ray, scene, this->scene->sceneObjects[next_intersecting_index], remaining_hop-1, next_intersecting_index, inside), target_obj->getMaterial()->mirrorReflectance),fr);
        }

        minTValue = 9999999;
        next_intersecting_index = -1;
        
        // we return sum of the two
        std::vector<float> cur_rad = vectorScale(vectorMultiplyElementwise(target_obj->getMaterial()->mirrorReflectance, lx), fr);
        return vectorAdd(vectorAdd(cur_rad, transmitted_return), reflected_return) ;
    }
    else if (target_obj->getMaterial()->materialType == MaterialType::Conductor){

        std::vector<float> lx = this->radianceAt(intersecting_location, target_obj, intersect_index);


        float k2 = 0;
        if (!inside)
            k2 = target_obj->getMaterial()->absorbtion_index;
        float rs = ((pow(n2, 2) + pow(k2,2)) - 2*n2*cos_theta + pow(cos_theta,2)) /  ((pow(n2, 2) + pow(k2,2)) + 2*n2*cos_theta + pow(cos_theta,2)) ;
        float rp = ((pow(n2, 2) + pow(k2,2))*pow(cos_theta,2) - 2 * n2 * cos_theta +1) / ((pow(n2, 2) + pow(k2,2))*pow(cos_theta,2) + 2 * n2 * cos_theta +1);
        float fr = (rs+rp)/2;
        float ft = 0;


        // we can optimize here TODO:
        // now we calculated current effect we need to determine if the next ray hits another object. If so we recurse if not we terminate here
        float minTValue = 9999999;
        int next_intersecting_index = -1;
        
        for (int k = 0; k < this->scene->numObjects; k++)
        {
            float tvalue = this->scene->sceneObjects[k]->Intersects(reflected_ray);
            if (tvalue > 0 && tvalue < minTValue){
                minTValue = tvalue;
                next_intersecting_index = k;
            }
        }
            

        if (next_intersecting_index == -1){   
            // we do not need to recurse
            return vectorScale(vectorMultiplyElementwise(target_obj->getMaterial()->mirrorReflectance, lx), fr) ;
        }


        return vectorAdd(vectorScale(vectorMultiplyElementwise(target_obj->getMaterial()->mirrorReflectance, lx), fr) , 
        vectorScale(vectorMultiplyElementwise(refractionTransmission(reflected_ray, scene, this->scene->sceneObjects[next_intersecting_index], remaining_hop-1, next_intersecting_index, inside), target_obj->getMaterial()->mirrorReflectance), fr));
    }

    
    
    
    

    








}

std::vector<float> Shader::specularReflection(Ray r, Scene* scene, Object* target_obj, int remaining_hop, int intersect_index){
    /*
    Sends recursive rays in order to make reflection. First Calculates if given ray hits an object if so returns coef * {color of given location}

    1. move ray a little to its direction
    2. Find the intersection
    3. Calculate the shading at location
    4. return coef*calculation with remaining_hop decremented
    */
   vector<float> diffuse_intensity = this->diffuseShadingAt(r.locationAtT(target_obj->Intersects(r)), target_obj, intersect_index);
    vector<float> ambient_intensity = this->ambientShadingAt(r.locationAtT(target_obj->Intersects(r)), target_obj, intersect_index);
    vector<float> specular_intensity = this->specularShadingAt(r, r.locationAtT(target_obj->Intersects(r)) ,scene->sceneObjects[intersect_index], intersect_index);
    vector<float> current_pixel_val = vectorAdd(specular_intensity, vectorAdd(diffuse_intensity, ambient_intensity));
    
    if (remaining_hop == 0 || target_obj->getMaterial()->materialType != MaterialType::Mirror){
        return current_pixel_val;
    }

    
    std::vector<float> n = scene->sceneObjects[intersect_index]->getSurfaceNormal(r.locationAtT(target_obj->Intersects(r)));
    // TODO: is this r correct ? 
    float cos_theta = dotProduct(n, vectorScale(r.d, -1));
    Ray new_r = Ray(r.locationAtT(target_obj->Intersects(r)),  vectorAdd(r.d, vectorScale(n, 2 * cos_theta)));
    new_r.o = vectorAdd(new_r.o, vectorScale(new_r.d, 1 + scene->shadow_ray_eps));

    // 2
    float mint_value = 999999;
    int new_intersect_index = -1;
    for (int k = 0; k < scene->numObjects; k++)
    {
        float tvalue = scene->sceneObjects[k]->Intersects(new_r);
        if (tvalue > 0 && tvalue < mint_value){
            mint_value = tvalue;
            new_intersect_index = k;
        }
    }

    if (new_intersect_index == -1){
        return current_pixel_val;
    }
    std::vector<float> child_color = specularReflection(new_r, scene, scene->sceneObjects[new_intersect_index], remaining_hop -1, new_intersect_index);
    // std::cout << "Child color this should be green: " << child_color.at(0) <<  " , "<< child_color.at(1) <<  " , "<< child_color.at(2) <<  "\n";
    return 
    vectorAdd(
    current_pixel_val ,
    vectorMultiplyElementwise(
     child_color, 
    target_obj->getMaterial()->mirrorReflectance
    )
    );

}



bool Shader::lightHits(Ray light_ray, std::vector<float> location, Object* intersectingObject, int intersectingObjIndex, Object** other_objects, int numobj ){
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


