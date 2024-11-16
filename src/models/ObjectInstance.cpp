
#include "ObjectInstance.h"
#include "Object.h"

ObjectInstance::ObjectInstance(Object* parent, bool reset)
{
this->parent = parent;
this->reset = reset;
std::cout << "I am creating an instance...";
}

ObjectInstance::~ObjectInstance()
{
}

    float ObjectInstance::Intersects(Ray ray){
        // TODO update here
        ray.o = this->tm->inverse().transform(ray.o);
        ray.d = this->tm->inverse().transform(ray.o);
        // for normalization
        Ray * new_ray = new Ray(ray.o, ray.d);
        return parent->Intersects(*new_ray);
    }
   
    Vec3 ObjectInstance::getSurfaceNormal(Ray r){
        r.o = this->tm->inverse().transform(r.o);
        r.d = this->tm->inverse().transform(r.o);
        // for normalization
        Ray * new_ray = new Ray(r.o, r.d);
        Vec3 parent_normal = parent->getSurfaceNormal(*new_ray);
        return this->tm->inverse().transpose().transform(parent_normal);
        


    }
    Material * ObjectInstance::getMaterial(){
        return this->material;
    }
    ObjectType ObjectInstance::getObject() {
        return parent->getObject();
    }
    Vec3 ObjectInstance::getBoundingBox(bool isMax){
        return this->tm->transform(parent->getBoundingBox(isMax));
    }
    Vec3  ObjectInstance::getCenter(){
        return this->tm->transform(parent->getCenter());
    }