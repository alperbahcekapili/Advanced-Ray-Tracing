#pragma once
#include <vector>
enum MaterialType {
    Mirror,
    Conductor,
    Dielectric
};

class Material{
    public:
    MaterialType materialType;
    std::vector<float>ambientProp;
    std::vector<float>diffuseProp;
    std::vector<float>specularProp;
    std::vector<float>mirrorReflectance;
    float refraction_index;
    float absorbtion_index;
    // make below rgb
    std::vector<float> absorption_coef;
    Material(MaterialType type, std::vector<float>ambientProp, std::vector<float>diffuseProp, std::vector<float>specularProp, float phong_exponent, std::vector<float> mirrorReflectance, float refraction_index, float absorbtion_index, std::vector<float> absorption_coef);
    float phong_exponent;

};