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
    Material(MaterialType type, float ambient[3], float diffuse[3], float specular[3], float phong_exponent);
    float phong_exponent;

};