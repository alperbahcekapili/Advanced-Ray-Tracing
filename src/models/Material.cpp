#include "Material.h"
#include <vector>
#include <iostream>
Material::Material(MaterialType type, std::vector<float>ambientProp, std::vector<float>diffuseProp, std::vector<float>specularProp, float phong_exponent, std::vector<float> mirrorReflectance, float refraction_index, float absorbtion_index, std::vector<float> absorption_coef):
phong_exponent(phong_exponent){
    this->materialType = type;
    this->ambientProp = ambientProp;
    this->diffuseProp = diffuseProp;
    this->specularProp = specularProp;
    this->mirrorReflectance = mirrorReflectance;
    // std::cout << "Mirror reflectance:" << mirrorReflectance.at(0) << ", " << "Mirror reflectance:" << mirrorReflectance.at(1) << ", "<< "Mirror reflectance:" << mirrorReflectance.at(2) << "\n";
    this->refraction_index = refraction_index;
    this->absorbtion_index = absorbtion_index;
    this->absorption_coef = absorption_coef;
}