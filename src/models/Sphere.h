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
    
    Sphere(Vec3 center, float R, Material* material, ObjectType objectType, TransformationMatrix* tm);
    float Intersects(Ray ray) override ;
    Vec3 getSurfaceNormal(Ray r) override;
    Material* getMaterial() override;
    TransformationMatrix* gettm() override;
    ObjectType getObject() override;
    Vec3 getBoundingBox(bool isMax) override;
    Vec3  getCenter() override;
Vec3 getMotionBlur() override;
    
    
};

