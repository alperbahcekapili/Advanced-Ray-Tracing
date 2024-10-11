#include "Material.h"
#include <vector>

Material::Material(MaterialType type, float ambient[3], float diffuse[3], float specular[3]){
    this->materialType = type;
    this->ambientProp = std::vector<float>(ambient, ambient+3);
    this->diffuseProp = std::vector<float>(diffuse, diffuse+3);
    this->specularProp = std::vector<float>(specular, specular+3);
}