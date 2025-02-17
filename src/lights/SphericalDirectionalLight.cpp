#include "SphericalDirectionalLight.h"
#include "../shaders/TextureImage.h"


SphericalDirectionalLight::SphericalDirectionalLight(TextureImage* image){
    this->image = image;
    this->latlong = image->width > image->height;
    this->ltype = SphericalDirectionalLightType;
}



// light_ray will be the one that is sampled randomly for object, or primary ray direction for bg
Vec3  SphericalDirectionalLight::irradianceAt(Ray light_ray, Vec3  location)  {
    

    if(latlong){
        float u = 1 + (atan2(light_ray.d.x  , -light_ray.d.z) / M_PI);
        float v = acos(light_ray.d.y)/M_PI;
        Vec3 pixval = this->image->get_value(u/2,v,BILINEAR) * 255.0f;;
        return  pixval;// * 255 because I scale in textureimage before returning
    }
    float r = (1/M_PI) * acos(-light_ray.d.z)/pow(pow(light_ray.d.x,2)+pow(light_ray.d.y,2), 0.5) ;
    float u = light_ray.d.x * r ;
    float v = light_ray.d.y * r;
    Vec3 pixel_val = this->image->get_value((u+1)/2,(v+1)/2,BILINEAR) * 255.0f;
    return pixel_val;


}   

Vec3 SphericalDirectionalLight::getPointOn(){
    return this->location;
}
