#include "Material.h"
#include <vector>
#include <iostream>
#include "../util/data_structures.h"

Material::Material(MaterialType type, Vec3 ambientProp, Vec3 diffuseProp, Vec3 specularProp, float phong_exponent, Vec3  mirrorReflectance, float refraction_index, float absorbtion_index, Vec3  absorption_coef):
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