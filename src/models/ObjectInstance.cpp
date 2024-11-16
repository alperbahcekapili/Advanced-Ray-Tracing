
#include "ObjectInstance.h"
#include "Object.h"

ObjectInstance::ObjectInstance(Object* parent, bool reset, TransformationMatrix* tm)
{
this->parent = parent;
this->reset = reset;
std::cout << "I am creating an instance...";
this->tm = tm;

if(reset){
    *(this->tm) = *(this->tm) * (parent->tm->inverse()) ;
}
this->max = this->tm->transform(parent->getBoundingBox(true));
this->min = this->tm->transform(parent->getBoundingBox(false));

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
        r.d = this->tm->inverse().transform(r.d);
        // for normalization
        Ray * new_ray = new Ray(r.o, r.d);
        Vec3 parent_normal = parent->getSurfaceNormal(*new_ray);
        return this->tm->inverse().transpose().transform(parent_normal);
        


    }
    Material * ObjectInstance::getMaterial(){
        return this->parent->getMaterial();
    }
    ObjectType ObjectInstance::getObject() {
        return parent->getObject();
    }
    Vec3 ObjectInstance::getBoundingBox(bool isMax){

    if(isMax)
        return this->max;
    return this->min;
    }
    Vec3  ObjectInstance::getCenter(){
        return this->tm->transform(parent->getCenter());
    }