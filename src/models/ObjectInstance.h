#pragma once
#include "../util/data_structures.h"
#include "Object.h"

class ObjectInstance : public Object
{
private:
    /* data */
public:


    Object* parent;
    bool reset; // reset transform of parent object

    TransformationMatrix* tm;
    float Intersects(Ray ray) override ;
    float getArea(void);
    Vec3 getSurfaceNormal(Ray r) override;
    Material *getMaterial() override;
    ObjectType getObject() override;
    Vec3 getBoundingBox(bool isMax) override;
    Vec3  getCenter() override;
    ObjectInstance(Object* parent, bool reset);
    ~ObjectInstance();
};
