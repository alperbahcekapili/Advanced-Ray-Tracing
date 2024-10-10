#include <vector>
enum MaterialType {
    Mirror,
    Conductor,
    Dielectric
};

class Material{
    public:
    MaterialType materialType;
    std::vector<int>ambientProp;
    std::vector<int>diffuseProp;
    std::vector<int>specularProp;
    Material(MaterialType type, int ambient[3], int diffuse[3], int specular[3]);


};