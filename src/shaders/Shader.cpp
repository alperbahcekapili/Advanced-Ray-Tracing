#include "Shader.h"
#include "../models/Object.h"
#include "../models/Sphere.h"
#include "../models/Triangle.h"
#include "../models/Material.h"
#include "../util/util.h"
#include <math.h>
#include <stdio.h>

Shader::Shader(Scene* scene)
{
    this->scene = scene;

}

Shader::~Shader()
{
}



Vec3  Shader::radianceAt(Vec3  location, Object* intersectingObject, int intersectingObjIndex){
    Vec3  radiance(0,0,0);
    for (int i = 0; i < this->scene->numlights; i++)
        {

            
            Ray lightRay = createLightRay(this->scene->lights[i], location);
            
            bool ligth_hits = lightHits(lightRay, location, intersectingObject, intersectingObjIndex, this->scene->sceneObjects, this->scene->numObjects)    ;;
            if (!ligth_hits)
                continue;
            Vec3 n = intersectingObject->getSurfaceNormal(lightRay);
            // If we got so far then it means ith light source hits this surface thus we can calculate illumination
            float cosTheta = lightRay.d.dot(n);
            Vec3  irradiance = this->scene->lights[i]->irradianceAt(lightRay, location) * cosTheta;
            radiance = irradiance + radiance;
        }

    return radiance;
}   


/*
This function calculates refraction and transmission
*/
Vec3  Shader::refractionTransmission(Ray r, Scene* scene, Object* target_obj, int remaining_hop, int intersect_index){
    float intersecting_t = target_obj->Intersects(r);
    Vec3  intersecting_location = r.locationAtT(intersecting_t);


    // The reflected ray direction is same in conductor and dielectric types
    Vec3  n = target_obj->getSurfaceNormal(r);
    Material* target_mat = target_obj->getMaterial();


    Vec3  diffuse_intensity = this->diffuseShadingAt(intersecting_location, target_obj, intersect_index);
    Vec3  ambient_intensity = this->ambientShadingAt(intersecting_location, target_obj, intersect_index);
    Vec3  specular_intensity = this->specularShadingAt(r, intersecting_location ,target_obj, intersect_index);
    Vec3  pixel_val = specular_intensity + diffuse_intensity + ambient_intensity;
    // if hop left is 0 then I need to return my color
    if (remaining_hop < 1){
        // std::cout << "Remanining hop is over I return my color : " << pixel_val.x  << ", "  << pixel_val.y  << ", "  << pixel_val.z  << "\n" ;
        return pixel_val;          
    }
    if(target_mat->materialType != MaterialType::Dielectric && target_mat->materialType != MaterialType::Conductor){
        // std::cout << "I am not dielectric or conductor : " << pixel_val.x  << ", "  << pixel_val.y  << ", "  << pixel_val.z  << "\n" ;
        // std::cout << "My index is : " << intersect_index << "\n";
        return pixel_val;}


    // Now that we calculated transmitted ray and reflected ray and their coefficents we can recurse. First let us check if they hit another object
    Vec3 reflected_return(0,0,0);
    Vec3 transmitted_return(0,0,0);
    Vec3 reflected_return2(0,0,0);
    // std::cout << "n: " << n.x  << ", "<< n.y  << ", " << n.z  << "\n";
    float cos_theta = (r.d * -1).dot(n);
    // costhetat should not be negative here
    // cos_theta = abs(cos_theta);
    float n1 = scene->refraction_index;
    float n2 = target_obj->getMaterial()->refraction_index;
    float before_sqrt = 1 - (pow(n1 / n2, 2) * (1 - pow(cos_theta, 2)));
    Ray reflected_ray = Ray(intersecting_location, r.d + (n * 2 * cos_theta));
    // TODO replace this with shadow eps
    reflected_ray.o = reflected_ray.o + (reflected_ray.d * scene->shadow_ray_eps);
    // If roughness parameter is other than zero then we need to deviate from original direction a little
    if(target_mat->roughness != 0){
        float deviation_magnx = generate_random_01();
        float deviation_magny = generate_random_01();
        Vec3 ** tangents = reflected_ray.getTangentVectors();
        Vec3 dev1 = target_mat->roughness * deviation_magnx * (*tangents[0]);
        Vec3 dev2 = target_mat->roughness * deviation_magny * (*tangents[1]);
        reflected_ray.d = reflected_ray.d + dev1 + dev2;
    }
    // We need to move reflected ray along its direction in order to prevent self intersection

    if (target_obj->getMaterial()->materialType == MaterialType::Dielectric){
        
        float cosphi = sqrt(before_sqrt);
        

        // first transmission ray is going to inside of the object
        Ray transmitted_ray = Ray(intersecting_location, (((n*cos_theta) + r.d) * (n1/n2)) - (n*cosphi));
        transmitted_ray.o = transmitted_ray.o + (transmitted_ray.d * scene->shadow_ray_eps);
        float rpar = (n2 * cos_theta - n1 * cosphi) / (n2 * cos_theta + n1 * cosphi);
        float rper = (n1 * cos_theta - n2 * cosphi) / (n1 * cos_theta + n2 * cosphi);
        float fr = (pow(rpar,2) + pow(rper,2)) / 2;
        float ft = 1 - fr;

        fr = fmin(fmax(fr, 0), 1);
        ft = fmin(fmax(ft, 0), 1);


        // Now we calculated where does transmitted ray go inside the object and what is the ratio of reflection and transmission
        // We will use them in ourrecursive calls
        if(before_sqrt < 0){
            cosphi = 0;
            fr = 1.0f;
            ft = 0.0f;

        }
        // std::cout << "rpar: " << rpar << ", rper: " << rper << "\n";
        // std::cout << "beforesqrt: " << before_sqrt << "\n";
        // std::cout << "coshtheta: " <<cos_theta << ", cosphi: "<< cosphi << "\n";
        // std::cout << "n1: " << n1 << ", n2: " << n2 << "\n";
        // std::cout << "fr: " << fr << ", ft: " << ft << "\n";
        // Let us calculate where does trasmitted ray hit to object
        Vec3  l0 = this->radianceAt(intersecting_location, target_obj, intersect_index);
        float inside_hit_location_t = target_obj->Intersects(transmitted_ray);
        Vec3  inside_hit_location = transmitted_ray.locationAtT(inside_hit_location_t + scene->shadow_ray_eps);
       
        
        Ray outgoing_ray = Ray(inside_hit_location, transmitted_ray.d);
        // now let us calculate attenuation magnitude

        Vec3 inside_normal = target_obj->getSurfaceNormal(transmitted_ray);
        float inside_cos = (transmitted_ray.d * -1).dot(inside_normal); // we negate the normal TODO need remove negate ?
        float before_sqrt_internal = 1-(pow(n2/n1, 2)*(1-pow(inside_cos,2)));
        outgoing_ray = Ray(inside_hit_location, (1+this->scene->shadow_ray_eps)*(transmitted_ray.d+n*inside_cos)*(n2/n1) - n*sqrt(before_sqrt_internal));
        if(target_mat->roughness != 0){
            float deviation_magnx = generate_random_01();
            float deviation_magny = generate_random_01();
            Vec3 ** tangents = outgoing_ray.getTangentVectors();
            Vec3 dev1 = target_mat->roughness * deviation_magnx * (*tangents[0]);
            Vec3 dev2 = target_mat->roughness * deviation_magny * (*tangents[1]);
            outgoing_ray.d = outgoing_ray.d + dev1 + dev2;
        }


        
        // attenuate
        // we will mutliply this with outgoint ray outcome
        float x = sqrt((intersecting_location - inside_hit_location).magnitude());
        Vec3  attenuation_magn(0,0,0);
        Vec3  mincx = target_obj->getMaterial()->absorption_coef * -x;
        attenuation_magn.x  = exp(mincx.x );
        attenuation_magn.y  = exp(mincx.y );
        attenuation_magn.z  = exp(mincx.z );

        


        // we start 2 calls for dielectric one for transmission one for refraction
        // we can optimize here TODO:
        // now we calculated current effect we need to determine if the next ray hits another object. If so we recurse if not we terminate here
        float minTValue = 9999999;
        int next_intersecting_index = -1;
        Object* tofill = nullptr;
        float  maxTValue;
        bool intersected = scene->bvh->intersectObject(outgoing_ray, tofill, minTValue, maxTValue);
        if(intersected)
            next_intersecting_index = tofill -> id;
            
        if (next_intersecting_index != -1){
            Vec3  coeff = attenuation_magn;
            coeff = coeff * ft ;
            // std::cout << "\n ft: " << ft << "\n";
            // std::cout << "I am dielectric, I multiply transmission recursion output with: " << coeff.x  << "," << coeff.y  << "," << coeff.z  << "\n" ;
            transmitted_return = refractionTransmission(outgoing_ray, scene, tofill, remaining_hop-1, next_intersecting_index) * coeff;
            // std::cout << "Transmission output: " << transmitted_return.x << ", " << transmitted_return.y << ", " << transmitted_return.z << "\n";
        }


        next_intersecting_index = -1;
        Object* tofill2 = nullptr;
        float minTValue2, maxTValue2;
        intersected = scene->bvh->intersectObject(reflected_ray, tofill2, minTValue2, maxTValue2);
        if(intersected)
            next_intersecting_index = tofill2 -> id;
            
        
        if (next_intersecting_index != -1){
            Vec3  coeff = Vec3(fr,fr,fr);
            // printf("Fr: %f\n", fr);
            // std::cout << "I am dielectric, I multiply reflection recursion output with: " << coeff.x  << "," << coeff.y  << "," << coeff.z  << "\n" ;
            reflected_return = refractionTransmission(reflected_ray, scene, tofill2, remaining_hop-1, next_intersecting_index) * coeff;
        }
        return pixel_val + transmitted_return + reflected_return ;
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
        
        Object* tofill = nullptr;
        float  maxTValue;
        bool intersected = scene->bvh->intersectObject(reflected_ray, tofill, minTValue, maxTValue);
        if(intersected)
            next_intersecting_index = tofill -> id;
            

        if (next_intersecting_index == -1){   
            // we do not need to recurse
            return pixel_val;
        }
        
        Vec3  coeff  =target_obj->getMaterial()->mirrorReflectance * fr;
        // std::cout << "I am conductor fr: " << fr << "\n";
        // std::cout << "I am conductor , mirror reflectance: " << target_obj->getMaterial()->mirrorReflectance.x  << "," << target_obj->getMaterial()->mirrorReflectance.y  << ","<< target_obj->getMaterial()->mirrorReflectance.z  << "\n";
        // std::cout << "I am conductor, I multiply reflection recursion output with: " << coeff.x  << "," << coeff.y  << "," << coeff.z  << "\n" ;
        return pixel_val + (refractionTransmission(reflected_ray, scene, tofill, remaining_hop-1, next_intersecting_index) * coeff);
            
    }


}

Vec3  Shader::specularReflection(Ray r, Scene* scene, Object* target_obj, int remaining_hop, int intersect_index){
    Vec3  diffuse_intensity = this->diffuseShadingAt(r.locationAtT(target_obj->Intersects(r)), target_obj, intersect_index);
    Vec3  ambient_intensity = this->ambientShadingAt(r.locationAtT(target_obj->Intersects(r)), target_obj, intersect_index);
    Vec3  specular_intensity = this->specularShadingAt(r, r.locationAtT(target_obj->Intersects(r)) ,scene->sceneObjects[intersect_index], intersect_index);
    Vec3  current_pixel_val = specular_intensity + diffuse_intensity + ambient_intensity;

    // std::cout << "Ambient Intensity: \n" <<  ambient_intensity.x << ", " << ambient_intensity.y << ", " << ambient_intensity.z << "\n";
    Material* target_mat = target_obj->getMaterial();
    if(target_mat->materialType == MaterialType::Dielectric || target_mat->materialType == MaterialType::Conductor){
        // std::cout << "Target is dielectric \n";
        return refractionTransmission(r, scene, target_obj, remaining_hop-1, intersect_index);
    }
                
    if (remaining_hop == 0 || target_mat->materialType != MaterialType::Mirror){
        // std::cout << "I return: " << current_pixel_val.x  << ", " << current_pixel_val.y  << ", " << current_pixel_val.z  << "\n";
        return current_pixel_val;
    }

    // std::cout << "Target is mirror!";
    
    Vec3  n = scene->sceneObjects[intersect_index]->getSurfaceNormal(r);
    // TODO: is this r correct ? 
    float cos_theta = n.dot( r.d * -1);
    Ray new_r = Ray(r.locationAtT(target_obj->Intersects(r)),  r.d + ( n * 2 * cos_theta));
    new_r.o = new_r.o + (new_r.d * scene->shadow_ray_eps);

    // If roughness parameter is other than zero then we need to deviate from original direction a little
    if(target_mat->roughness != 0){
        float deviation_magnx = generate_random_01();
        float deviation_magny = generate_random_01();
        Vec3 ** tangents = new_r.getTangentVectors();
        Vec3 dev1 = target_mat->roughness * deviation_magnx * (*tangents[0]);
        Vec3 dev2 = target_mat->roughness * deviation_magny * (*tangents[1]);
        new_r.d = new_r.d + dev1 + dev2;
    }


    
    
    // 2
    float mint_value = 999999;
    int new_intersect_index = -1;
    Object* tofill = nullptr;
    float minTValue, maxTValue;
    bool intersected = scene->bvh->intersectObject(new_r, tofill, minTValue, maxTValue);
    if(intersected)
        new_intersect_index = tofill -> id;

    if (new_intersect_index == -1){
        return current_pixel_val;
    }
    Vec3  child_color = specularReflection(new_r, scene, scene->sceneObjects[new_intersect_index], remaining_hop -1, new_intersect_index);
    // std::cout << "Child color this should be green: " << child_color.x  <<  " , "<< child_color.y  <<  " , "<< child_color.z  <<  "\n";
    return  current_pixel_val + ( child_color * target_obj->getMaterial()->mirrorReflectance );

}



bool Shader::lightHits(Ray light_ray, Vec3  location, Object* intersectingObject, int intersectingObjIndex, Object** other_objects, int numobj ){
    // create a ray from the lightsource to destination
    
    // now we need to test if this ray intersects with any of the objects
    // if ray intersects with any objects other than intersecting object with smaller t value then this area
    // is not lighted by this lightsource
    
    float intersectingTvalue = intersectingObject->Intersects(light_ray);
    // std::cout << "Light hits the object in required area. with thvalue:"<< intersectingTvalue <<  "..\n"; 
    
    // we can calculate intersecting location with this tvalue and it is different 
    // from the given location then that means light hits the other side of the object
    Vec3  lightHitLocation = light_ray.locationAtT(intersectingTvalue);
    float errorMargin = 0.01;
    if(((lightHitLocation - location )).magnitude() > errorMargin){
        // This means the light is in the other side of the object
        // std::cout << "Light is on the other side. for "<< intersectingObjIndex << " \n";
        return false;
    }

    // if(intersectingObject->getMotionBlur().z!= 0){
    //     int b = -1;
    //     printf("%d", b);
    // }
    bool in_shadow = false;
    float new_tmin, new_tmax;
    Object* tmp;
    bool bvh_int = this->scene->bvh->intersectObject(light_ray, tmp, new_tmin, new_tmax);
    if(new_tmin < intersectingTvalue)
        in_shadow=true;
    return !in_shadow;

       
}


Vec3 Shader::diffuseShadingAt(Vec3  location, Object* intersectingObject, int intersectingObjIndex){

    /*
    Iterate over the lightsources and detect if the location is lightened by the lightsource
    */
   
   // In order to preserve the triangle that we hit at the beginning I save it in a new var.
   Triangle* hitTri = nullptr;

   Vec3  pixel(0,0,0);
   for (int i = 0; i < this->scene->numlights; i++)
   {
        Ray lightRay = createLightRay(this->scene->lights[i], location);
        
        bool ligth_hits = lightHits(lightRay, location, intersectingObject, intersectingObjIndex, this->scene->sceneObjects, this->scene->numObjects)    ;;
        if (!ligth_hits){        
            // printf("Light does not hit obejct\n");
            continue;}

        

        float cosTheta = (lightRay.d * -1).dot(intersectingObject->getSurfaceNormal(lightRay) );
        if (cosTheta < 0)
            cosTheta*=-1; // TODO: update here
        
        Vec3  irradiance = this->scene->lights[i]->irradianceAt(lightRay, location) * cosTheta;
        // std::cout << "Irradiance: " << irradiance.x  << "," << irradiance.y  << "," << irradiance.z  <<  "\n";
        Vec3  tmp = intersectingObject->getMaterial()->diffuseProp * irradiance;

        
        if(intersectingObject->getObject() == MeshType){
            Mesh* mesh = dynamic_cast<Mesh*>(intersectingObject);
            if(hitTri == nullptr)
                hitTri = dynamic_cast<Triangle*>(mesh->last_intersected_obj);
            Triangle* triangle = hitTri;
            uv tmp_uv = uv::calculateUVTriangle(location, triangle->v1, triangle->v2, triangle->v3, 
            triangle->uv_coords_triangle.at(0), triangle->uv_coords_triangle.at(1), triangle->uv_coords_triangle.at(2));
            Vec3 bg_pixel_val = Vec3(0,0,0);
            if(triangle->get_texture_flags().replace_kd_texture->is_image)
                bg_pixel_val = triangle->get_texture_flags().replace_kd_texture->interpolateAt(tmp_uv, triangle->get_texture_flags().replace_kd_texture->interpolation_type);
            else{
                Vec3 scene_min = this->scene->bvh->min;
                Vec3 scene_max = this->scene->bvh->max;
                Vec3 corners[8];
                corners[0] = Vec3(scene_min.x, scene_min.y, scene_min.z);
                corners[1] = Vec3(scene_min.x, scene_min.y, scene_max.z);
                corners[2] = Vec3(scene_min.x, scene_max.y, scene_min.z);
                corners[3] = Vec3(scene_min.x, scene_max.y, scene_max.z);
                corners[4] = Vec3(scene_max.x, scene_min.y, scene_min.z);
                corners[5] = Vec3(scene_max.x, scene_min.y, scene_max.z);
                corners[6] = Vec3(scene_max.x, scene_max.y, scene_min.z);
                corners[7] = Vec3(scene_max.x, scene_max.y, scene_max.z);
                bg_pixel_val = triangle->get_texture_flags().replace_kd_texture->interpolateAt(tmp_uv, location, corners, scene_min, scene_max, NEAREAST_NEIGHBOR);
                
            }
            if(intersectingObject->get_texture_flags().replace_kd)
                tmp =  bg_pixel_val * irradiance;
            else if(intersectingObject->get_texture_flags().blend_kd){
                tmp = tmp + (bg_pixel_val * irradiance);
                tmp = tmp / 2;
            }else if(intersectingObject->get_texture_flags().replace_all){
                bg_pixel_val = triangle->get_texture_flags().replace_all_texture->interpolateAt(tmp_uv, triangle->get_texture_flags().replace_all_texture->interpolation_type) * 255;
                return bg_pixel_val;
            }   
        }
        
        pixel = pixel + tmp;
    }
    return pixel;
}



Vec3  Shader::ambientShadingAt(Vec3  location, Object* intersectingObject, int intersectingObjIndex){
    Vec3  resultingMagnitude = Vec3(0,0,0);
    // if this object has replace_all texture it is calculated in diffuse property so terminate here directly
    if(intersectingObject->get_texture_flags().replace_all)
        return resultingMagnitude;

    return intersectingObject->getMaterial()->ambientProp * this->scene->ambient_light;
}



Vec3  Shader::specularShadingAt(Ray cameraRay,Vec3  location, Object* intersectingObject, int intersectingObjIndex){
    Vec3  resultingMagnitude = Vec3(0,0,0);
    // if this object has replace_all texture it is calculated in diffuse property so terminate here directly
    if(intersectingObject->get_texture_flags().replace_all)
        return resultingMagnitude;


    for (int i = 0; i < this->scene->numlights; i++)
    {
        // create a ray from the lightsource to destination
        Ray lightRay = createLightRay(this->scene->lights[i], location);
        bool ligth_hits = lightHits(lightRay, location, intersectingObject, intersectingObjIndex, this->scene->sceneObjects, this->scene->numObjects)    ;;
        if (!ligth_hits)
            continue;

        Vec3  surface_normal = intersectingObject->getSurfaceNormal(lightRay);
        // std::cout <<  "Lightray in specular " <<lightRay.d.x  << "," << lightRay.d.z  << "," << lightRay.d.z  << "\n" ;
        
        Vec3  h = (cameraRay.d + lightRay.d).normalize();
        // std::cout <<  "Lightray in specular " <<h.x  << "," << h.z  << "," << h.z  << "\n" ;
        float costheta  = surface_normal.dot(h*-1);
        if (costheta < 0){
            costheta = 0;
        }
        else{
            costheta = pow(costheta, intersectingObject->getMaterial()->phong_exponent);
        }

        if(intersectingObject->get_texture_flags().replace_ks){
            if(intersectingObject->getObject() == MeshType){
                Mesh* mesh = dynamic_cast<Mesh*>(intersectingObject);
                Triangle* triangle = dynamic_cast<Triangle*>(mesh->last_intersected_obj);
                uv tmp_uv = uv::calculateUVTriangle(location, triangle->v1, triangle->v2, triangle->v3, 
                triangle->uv_coords_triangle.at(0), triangle->uv_coords_triangle.at(1), triangle->uv_coords_triangle.at(2));
                Vec3 bg_pixel_val = triangle->get_texture_flags().replace_kd_texture->interpolateAt(tmp_uv, NEAREAST_NEIGHBOR);
                resultingMagnitude = resultingMagnitude + (bg_pixel_val * (this->scene->lights[i]->irradianceAt(lightRay, location) * costheta));
            } 
        }else{
            resultingMagnitude = resultingMagnitude + 
        (intersectingObject->getMaterial()->specularProp * 
        (this->scene->lights[i]->irradianceAt(lightRay, location) * costheta));
        }
        

        

    }

    return resultingMagnitude;

}


