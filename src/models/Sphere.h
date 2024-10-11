#include "Object.h"

class Sphere : public Object
{
private:
    
public:
    float cx;
    float cy;
    float cz;
    float R;
    Sphere(float cx, float cy, float cz, float R, Material* material, ObjectType objectType);
    float Intersects(Ray ray);
    ~Sphere();
};

