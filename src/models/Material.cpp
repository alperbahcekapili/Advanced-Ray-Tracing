#include "Material.h"
#include <vector>

Material::Material(MaterialType type, float ambient[3], float diffuse, float specular[3], std::vector<float> color){
    this->color = color;
    this->materialType = type;
    this->ambientProp = std::vector<float>(ambient, ambient+3);
    this->diffuseProp = diffuse;
    this->specularProp = std::vector<float>(specular, specular+3);
}