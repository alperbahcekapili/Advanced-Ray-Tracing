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
std::vector<float> Shader::refractionTransmission(Ray r, Scene* scene, Object* target_obj, int remaining_hop, int intersect_index){
    float intersecting_t = target_obj->Intersects(r);
    std::vector<float> intersecting_location = r.locationAtT(intersecting_t);
    
    vector<float> diffuse_intensity = this->diffuseShadingAt(intersecting_location, target_obj, intersect_index);
    vector<float> ambient_intensity = this->ambientShadingAt(intersecting_location, target_obj, intersect_index);
    vector<float> specular_intensity = this->specularShadingAt(r, intersecting_location ,target_obj, intersect_index);
    vector<float> pixel_val = vectorAdd(specular_intensity, vectorAdd(diffuse_intensity, ambient_intensity));
    // if hop left is 0 then I need to return my color
    if (remaining_hop == 0){
        std::cout << "Remanining hop is over I return my color : " << pixel_val.at(0) << ", "  << pixel_val.at(1) << ", "  << pixel_val.at(2) << "\n" ;
        return pixel_val;          
    }
    if(target_obj->getMaterial()->materialType != MaterialType::Dielectric && target_obj->getMaterial()->materialType != MaterialType::Conductor){
    // std::cout << "I am not dielectric or conductor : " << pixel_val.at(0) << ", "  << pixel_val.at(1) << ", "  << pixel_val.at(2) << "\n" ;
    // std::cout << "My index is : " << intersect_index << "\n";
        return pixel_val;}


    // The reflected ray direction is same in conductor and dielectric types
    std::vector<float> n = target_obj->getSurfaceNormal(intersecting_location);
    // std::cout << "n: " << n.at(0) << ", "<< n.at(1) << ", " << n.at(2) << "\n";
    float cos_theta = dotProduct(vectorScale(r.d, -1), n);
    float n1 = scene->refraction_index;
    float n2 = target_obj->getMaterial()->refraction_index;
    float before_sqrt = 1-(pow(n1/n2, 2)*(1-pow(cos_theta,2)));
    Ray reflected_ray = Ray(intersecting_location, vectorAdd(r.d , vectorScale(n, 2 * cos_theta)));
    // TODO replace this with shadow eps
    reflected_ray.o = vectorAdd(reflected_ray.o, vectorScale(reflected_ray.d, scene->shadow_ray_eps));
    // We need to move reflected ray along its direction in order to prevent self intersection

    if (target_obj->getMaterial()->materialType == MaterialType::Dielectric){
        float cosphi = sqrt(before_sqrt);
        // first transmission ray is going to inside of the object
        Ray transmitted_ray = Ray(intersecting_location, vectorSubstract(  vectorScale(vectorAdd(r.d , vectorScale(n, cos_theta)), n1/n2), vectorScale(n, cosphi) ) );
        transmitted_ray.o = vectorAdd(transmitted_ray.o, vectorScale(transmitted_ray.d, scene->shadow_ray_eps));
        float rpar = (n2 * cos_theta - n1 * cos_theta) / (n2 * cos_theta + n1 * cos_theta);
        float rper = (n1 * cos_theta - n2 * cosphi) / (n1 * cos_theta - n2 * cosphi);
        float fr = (pow(rpar,2) + pow(rper,2)) / 2;
        float ft = 1 - fr;
        // Now we calculated where does transmitted ray go inside the object and what is the ratio of reflection and transmission
        // We will use them in our recursive calls
        
        // Let us calculate where does trasmitted ray hit to object
        std::vector<float> l0 = this->radianceAt(intersecting_location, target_obj, intersect_index);
        float inside_hit_location_t = target_obj->Intersects(transmitted_ray);
        std::vector<float> inside_hit_location = transmitted_ray.locationAtT(inside_hit_location_t + scene->shadow_ray_eps);
        // now we know where the outgoing ray start from let us calculate outgoing ray as well
        // below assumes no fresnel reflection when exiting object TODO implement here
        // below assumes so angle change when exiting the obj TODO implement here
        Ray outgoing_ray = Ray(inside_hit_location, transmitted_ray.d);
        // now let us calculate attenuation magnitude
        
        // attenuate
        // we will mutliply this with outgoint ray outcome
        float x = sqrt(getMagnitude(vectorSubstract(intersecting_location, inside_hit_location)));
        std::vector<float> attenuation_magn = {0,0,0};
        std::vector<float> mincx = vectorScale(target_obj->getMaterial()->absorption_coef, -1*x);
        attenuation_magn.at(0) = exp(mincx.at(0));
        attenuation_magn.at(1) = exp(mincx.at(1));
        attenuation_magn.at(2) = exp(mincx.at(2));

        


        // Now that we calculated transmitted ray and reflected ray and their coefficents we can recurse. First let us check if they hit another object
        std::vector<float> reflected_return = {0,0,0};
        std::vector<float> transmitted_return = {0,0,0};
        // we start 2 calls for dielectric one for transmission one for refraction
        // we can optimize here TODO:
        // now we calculated current effect we need to determine if the next ray hits another object. If so we recurse if not we terminate here
        float minTValue = 9999999;
        int next_intersecting_index = -1;
        for (int k = 0; k < this->scene->numObjects; k++)
        {
            float tvalue = this->scene->sceneObjects[k]->Intersects(outgoing_ray);
            if (tvalue > 0 && tvalue < minTValue){
                minTValue = tvalue;
                next_intersecting_index = k;
            }
        }
        if (next_intersecting_index != -1){
            std::vector<float> coeff = attenuation_magn;
            coeff = vectorScale(coeff, ft);
            // std::cout << "I am dielectric, I multiply transmission recursion output with: " << coeff.at(0) << "," << coeff.at(1) << "," << coeff.at(2) << "\n" ;
            transmitted_return = 
                vectorMultiplyElementwise(
                    refractionTransmission(outgoing_ray, scene, this->scene->sceneObjects[next_intersecting_index], remaining_hop-1, next_intersecting_index), 
                    coeff);
                    
        }



        
        minTValue = 9999999;
        next_intersecting_index = -1;
        for (int k = 0; k < this->scene->numObjects; k++)
        {
            float tvalue = this->scene->sceneObjects[k]->Intersects(reflected_ray);
            if (tvalue > 0 && tvalue < minTValue){
                minTValue = tvalue;
                next_intersecting_index = k;
            }
        }
        
        if (next_intersecting_index != -1){
            std::vector<float> coeff = vectorScale({1,1,1},fr);
            // std::cout << "I am dielectric, I multiply reflection recursion output with: " << coeff.at(0) << "," << coeff.at(1) << "," << coeff.at(2) << "\n" ;
            reflected_return = 
            vectorMultiplyElementwise(
                refractionTransmission(reflected_ray, scene, this->scene->sceneObjects[next_intersecting_index], remaining_hop-1, next_intersecting_index), 
                coeff);
        }
        return vectorAdd(vectorAdd(pixel_val, transmitted_return), reflected_return) ;
    }
    else if (target_obj->getMaterial()->materialType == MaterialType::Conductor){
        
        float k2 = target_obj->getMaterial()->absorbtion_index;
        float rs = ((pow(n2, 2) + pow(k2,2)) - 2*n2*cos_theta + pow(cos_theta,2)) /  ((pow(n2, 2) + pow(k2,2)) + 2*n2*cos_theta + pow(cos_theta,2)) ;
        float rp = ((pow(n2, 2) + pow(k2,2))*pow(cos_theta,2) - 2 * n2 * cos_theta +1) / ((pow(n2, 2) + pow(k2,2))*pow(cos_theta,2) + 2 * n2 * cos_theta +1);
        float fr = (rs+rp)/2;
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
            return pixel_val;
        }
        
        std::vector<float> coeff  =vectorScale(target_obj->getMaterial()->mirrorReflectance, fr);
        // std::cout << "I am conductor fr: " << fr << "\n";
        // std::cout << "I am conductor , mirror reflectance: " << target_obj->getMaterial()->mirrorReflectance.at(0) << "," << target_obj->getMaterial()->mirrorReflectance.at(1) << ","<< target_obj->getMaterial()->mirrorReflectance.at(2) << "\n";
        // std::cout << "I am conductor, I multiply reflection recursion output with: " << coeff.at(0) << "," << coeff.at(1) << "," << coeff.at(2) << "\n" ;
        return vectorAdd(pixel_val,
            vectorMultiplyElementwise(
            refractionTransmission(reflected_ray, scene, this->scene->sceneObjects[next_intersecting_index], remaining_hop-1, next_intersecting_index), 
            coeff));
            
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
        // std::cout << "I return: " << current_pixel_val.at(0) << ", " << current_pixel_val.at(1) << ", " << current_pixel_val.at(2) << "\n";
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
    // float errorMargin = 0.01;
    // if(getMagnitude(vectorAdd(lightHitLocation, vectorScale(location, -1))) > errorMargin){
    //     // This means the light is in the other side of the object
    //     // std::cout << "Light is on the other side. for "<< intersectingObjIndex << " \n";
    //     return false;
    // }
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

    float cosTheta = dotProduct(vectorScale(lightRay.d, -1), intersectingObject->getSurfaceNormal(location) );
    if (cosTheta < 0)
        cosTheta = 0;
    std::vector<float> irradiance = vectorScale(this->scene->lights[i]->irradianceAt(location), cosTheta);

    //std::cout << "Irradiance: " << irradiance.at(0) << "," << irradiance.at(1) << "," << irradiance.at(2) <<  "\n";
    
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
        // std::cout <<  "Lightray in specular " <<lightRay.d.at(0) << "," << lightRay.d.at(2) << "," << lightRay.d.at(2) << "\n" ;
        
        std::vector<float> h = normalize(vectorAdd(cameraRay.d, lightRay.d));
        // std::cout <<  "Lightray in specular " <<h.at(0) << "," << h.at(2) << "," << h.at(2) << "\n" ;
        float costheta  = dotProduct(surface_normal,vectorScale(h,-1));
        if (costheta < 0){
            costheta = 0;
        }else{
            costheta = pow(costheta, intersectingObject->getMaterial()->phong_exponent);
        }


        resultingMagnitude = vectorAdd(resultingMagnitude,vectorMultiplyElementwise(intersectingObject->getMaterial()->specularProp, vectorScale(this->scene->lights[i]->irradianceAt(location),  costheta)));
    }

    return resultingMagnitude;

}


