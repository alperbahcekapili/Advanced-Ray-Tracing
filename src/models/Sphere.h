#include "Object.h"
#include <vector>

class Sphere : public Object
{
private:
    
public:
    Vec3 center;
    float R;
    Material* material;
    ObjectType objectType; 
    
    Sphere(Vec3 center, float R, Material* material, ObjectType objectType);
    float Intersects(Ray ray) override ;
    Vec3 getSurfaceNormal(Vec3 location) override;
    Material* getMaterial() override;
    ObjectType getObject() override;
    Vec3 getBoundingBox(bool isMax) override;
    Vec3  getCenter() override;

    
    
};

