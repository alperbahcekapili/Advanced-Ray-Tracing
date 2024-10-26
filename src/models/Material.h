#pragma once
#include <vector>
#include "../util/data_structures.h"
enum MaterialType {
    Mirror,
    Conductor,
    Dielectric,
    Other
};

class Material{
    public:
    MaterialType materialType;
    Vec3 ambientProp;
    Vec3 diffuseProp;
    Vec3 specularProp;
    Vec3 mirrorReflectance;
    float refraction_index;
    float absorbtion_index;
    Vec3  absorption_coef;
    Material(MaterialType type, Vec3 ambientProp, Vec3 diffuseProp, Vec3 specularProp, float phong_exponent, Vec3  mirrorReflectance, float refraction_index, float absorbtion_index, Vec3  absorption_coef);
    float phong_exponent;

};