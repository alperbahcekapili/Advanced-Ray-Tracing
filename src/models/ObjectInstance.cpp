
#include "ObjectInstance.h"
#include "Object.h"



TransformationMatrix* ObjectInstance::gettm() {
    return this->tm;
}


Vec3 ObjectInstance::getMotionBlur(){
    return this->motionBlur;
}




ObjectInstance::ObjectInstance(Object* parent, bool reset, TransformationMatrix* tm, Material* material, Vec3 motionBlur)
{
    this->parent = parent;
    this->reset = reset;
    this->motionBlur.x = motionBlur.x;
    this->motionBlur.y = motionBlur.y;
    this->motionBlur.z = motionBlur.z;
    std::cout << "I am creating an instance...";
    this->tm = new TransformationMatrix();
    TransformationMatrix blur_tra = TransformationMatrix(motionBlur, 't');
    this->material = material;
    if(reset){
        *(this->tm) = *(tm) * (parent->gettm()->inverse()) ;
    }else
    *(this->tm) = *(tm);




    Vec3 originalMin = parent->getBoundingBox(false);
    Vec3 originalMax = parent->getBoundingBox(true);

    // Get all 8 corners
    std::vector<Vec3> corners = {
        Vec3(originalMin.x, originalMin.y, originalMin.z),
        Vec3(originalMax.x, originalMin.y, originalMin.z),
        Vec3(originalMin.x, originalMax.y, originalMin.z),
        Vec3(originalMax.x, originalMax.y, originalMin.z),
        Vec3(originalMin.x, originalMin.y, originalMax.z),
        Vec3(originalMax.x, originalMin.y, originalMax.z),
        Vec3(originalMin.x, originalMax.y, originalMax.z),
        Vec3(originalMax.x, originalMax.y, originalMax.z),
    };

    // Transform corners and compute new AABB
    Vec3 newMin = this->tm->transform(corners[0]);
    Vec3 newMax = newMin;
    for (Vec3 corner : corners) {
        Vec3 transformed = this->tm->transform(corner);
        
        newMin.x = std::min(newMin.x, transformed.x);
        newMin.y = std::min(newMin.y, transformed.y);
        newMin.z = std::min(newMin.z, transformed.z);
        newMax.x = std::max(newMax.x, transformed.x);
        newMax.y = std::max(newMax.y, transformed.y);
        newMax.z = std::max(newMax.z, transformed.z);
    }

    this->min.x = newMin.x;
    this->min.y = newMin.y;
    this->min.z = newMin.z;
    this->max.x = newMax.x;
    this->max.y = newMax.y;
    this->max.z = newMax.z;


    Vec3 transformed_min = blur_tra.transform(min);
    Vec3 transformed_max = blur_tra.transform(max);
    this->min.x = std::min(this->min.x, transformed_min.x);
    this->min.y = std::min(this->min.y, transformed_min.y);
    this->min.z = std::min(this->min.z, transformed_min.z);

    this->max.x = std::max(this->max.x, transformed_max.x);
    this->max.y = std::max(this->max.y, transformed_max.y);
    this->max.z = std::max(this->max.z, transformed_max.z);


    std::cout << "My top coordinates (MMesh Instance): " << this->max.x << ", " << this->max.y << ", " << this->max.z << "\n";
    std::cout << "My bottom coordinates (MMesh Instance): " << this->min.x << ", " << this->min.y << ", " << this->min.z << "\n";

}

ObjectInstance::~ObjectInstance()
{
}

    float ObjectInstance::Intersects(Ray ray) {
        TransformationMatrix* temp_tm = new TransformationMatrix();
        if(this->motionBlur.x != 0 || this->motionBlur.y != 0 || this->motionBlur.z != 0)
            temp_tm = new TransformationMatrix(this->motionBlur*ray.time,'t');
        
        
    // Transform ray into the instance's local space
    Vec3 new_o = temp_tm->inverse().transform(ray.o);
    new_o = this->tm->inverse().transform(new_o);
    
    Vec3 new_d = this->tm->inverseUpperLeft3x3().transform(ray.d);
    Ray new_ray(new_o, new_d); // Use stack allocation

    // Check for intersection with the parent object
    float t_parent = parent->Intersects(new_ray);
    if (t_parent < 0) return -1;

    // Transform the intersection point back to world space
    Vec3 parent_int_location = new_ray.locationAtT(t_parent);
    Vec3 intersecting_location = this->tm->transform(parent_int_location);
    intersecting_location = temp_tm->transform(intersecting_location);
    

    // Calculate the distance from the original ray origin
    float resulting_t  = (new_o - intersecting_location).magnitude();
    
    return resulting_t;
    
}

   
    Vec3 ObjectInstance::getSurfaceNormal(Ray r) {
    // Transform ray direction into the instance's local space
    
    Vec3 new_o = this->tm->inverse().transform(r.o);
    Vec3 new_d = this->tm->inverseUpperLeft3x3().transform(r.d);
    Ray new_ray(new_o, new_d); // Use stack allocation


    // Get the surface normal from the parent object
    Vec3 parent_normal = parent->getSurfaceNormal(new_ray);

    // Transform the normal back to world space using the inverse transpose
    TransformationMatrix inv_tra = this->tm->inverseTransposeUpperLeft3x3();
    Vec3 world_normal = inv_tra.transform(parent_normal);

    // Normalize the world-space normal
    // return world_normal.normalize();
    
    return world_normal.normalize();
}

    Material * ObjectInstance::getMaterial(){
        return this->material;
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





    // TODO: Replace below, meshinstances might have their own textures:

int ObjectInstance::get_num_tex_maps(){
    return this->parent->num_tex_maps;
}
TextureMap* ObjectInstance::get_texture_maps() {
    return this->parent->texture_maps;
}
texture_flags ObjectInstance::get_texture_flags(){
    return this->parent->tex_flags;
}