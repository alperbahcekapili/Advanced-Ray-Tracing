#include "Object.h"
#include <vector>

class Sphere : public Object
{
private:
    
public:
    float cx;
    float cy;
    float cz;
    float R;
    Material* material;
    ObjectType objectType; 
    std::vector<float> center;
    Sphere(float cx, float cy, float cz, float R, Material* material, ObjectType objectType);
    float Intersects(Ray ray) override ;
    std::vector<float> getSurfaceNormal(std::vector<float> location) override;
    Material* getMaterial() override;
    ObjectType getObject() override;

    
    
};

