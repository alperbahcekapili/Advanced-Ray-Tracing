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
    float absorption_coef;
    Material(MaterialType type, float ambient[3], float diffuse[3], float specular[3], float phong_exponent, std::vector<float> mirrorReflectance, float refraction_index, float absorbtion_index, float absorption_coef);
    float phong_exponent;

};