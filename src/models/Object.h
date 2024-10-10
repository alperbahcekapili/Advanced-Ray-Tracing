#include "Ray.h"
#include "Material.h"

enum ObjectType{
    TriangleType,
    SphereType,
    MeshType
};

class Object
{
private:
    /* data */
public:

    bool Intersects(Ray ray);
    Material* material;
    ObjectType objectType;
    Object(Material* material, ObjectType objectType);
};