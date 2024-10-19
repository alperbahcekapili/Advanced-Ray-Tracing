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
    float diffuseProp;
    std::vector<float>specularProp;
    Material(MaterialType type, float ambient[3], float diffuse, float specular[3], std::vector<float> color, float phong_exponent);
    std::vector<float> color;
    float phong_exponent;

};