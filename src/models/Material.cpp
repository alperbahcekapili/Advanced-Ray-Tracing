#include "Material.h"
#include <vector>

Material::Material(MaterialType type, int ambient[3], int diffuse[3], int specular[3]){
    this->materialType = type;
    this->ambientProp = std::vector<int>(ambient, ambient+3);
    this->diffuseProp = std::vector<int>(diffuse, diffuse+3);
    this->specularProp = std::vector<int>(specular, specular+3);
}