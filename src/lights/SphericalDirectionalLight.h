#pragma once
#include "Light.h"
#include "../shaders/TextureImage.h"



class SphericalDirectionalLight : public Light
{
    public:
        TextureImage* image;
        bool latlong;  // o.w. 

        SphericalDirectionalLight(TextureImage* image);

        Vec3 irradianceAt(Ray light_ray, Vec3  location) override;
        Vec3 getPointOn() override;

};