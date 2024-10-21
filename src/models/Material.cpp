#include "Material.h"
#include <vector>

Material::Material(MaterialType type, float ambient[3], float diffuse[3], float specular[3], float phong_exponent, std::vector<float> mirrorReflectance, float refraction_index, float absorbtion_index, float absorption_coef):
phong_exponent(phong_exponent){
    this->materialType = type;
    this->ambientProp = std::vector<float>(ambient, ambient+3);
    this->diffuseProp = std::vector<float>(diffuse, diffuse+3);
    this->specularProp = std::vector<float>(specular, specular+3);
    this->mirrorReflectance = mirrorReflectance;
    this->refraction_index = refraction_index;
    this->absorbtion_index = absorbtion_index;
    this->absorption_coef = absorption_coef;
}