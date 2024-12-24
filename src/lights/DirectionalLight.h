#pragma once
#include "Light.h"


class DirectionalLight : public Light{
    private:
        Vec3 direction;
        Vec3 radiance;


    public: 

        DirectionalLight(Vec3 direction, Vec3 radiance);
        Vec3 getPointOn() override;
        Vec3 irradianceAt(Ray light_ray, Vec3  location) override;        
        Vec3 getDirection();
        Vec3 getRadiance();
};