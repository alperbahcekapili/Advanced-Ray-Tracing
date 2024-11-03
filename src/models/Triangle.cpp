
#include "Triangle.h"
#include "../util/util.h"
#include "../util/data_structures.h"
#include <math.h>


TransformationMatrix* Triangle::gettm(){
    return this->tm;
}
float Triangle::Intersects(Ray ray){
    Vec3 d = ray.d;
    Vec3 o = ray.o;
    /*
    
    */ 


    float gamma_top[3][3] = {
        {this->v1.x - this->v2.x, this->v1.x - o.x, d.x},
        {this->v1.y - this->v2.y, this->v1.y - o.y, d.y},
        {this->v1.z - this->v2.z, this->v1.z - o.z, d.z}
    };

    float beta_top[3][3] = {
        {this->v1.x - o.x, this->v1.x - this->v3.x, d.x},
        {this->v1.y - o.y, this->v1.y - this->v3.y, d.y},
        {this->v1.z - o.z, this->v1.z - this->v3.z, d.z}
    };

    float t_top[3][3] = {
        {this->v1.x - this->v2.x, this->v1.x - this->v3.x, this->v1.x - o.x},
        {this->v1.y - this->v2.y, this->v1.y - this->v3.y, this->v1.y - o.y},
        {this->v1.z - this->v2.z, this->v1.z - this->v3.z, this->v1.z - o.z}
    };

    float A[3][3] = {
        {this->v1.x - this->v2.x,this->v1.x - this->v3.x,d.x},
        {this->v1.y - this->v2.y,this->v1.y - this->v3.y,d.y},
        {this->v1.z - this->v2.z,this->v1.z - this->v3.z,d.z}
    };

    float beta_ = det3x3(beta_top) / det3x3(A);
    float gamma = det3x3(gamma_top) / det3x3(A);
    float t = det3x3(t_top) / det3x3(A);


    if(beta_ + gamma <= 1 && beta_ >= 0 && gamma >= 0)
        return t;

   return -1;


}

Vec3 Triangle::getSurfaceNormal(Ray r){
    return this->normal;
}

Triangle::Triangle(){

}
Vec3 Triangle::getMotionBlur(){
    return this->motionBlur;
}

Triangle::Triangle(Material* material, ObjectType objectType, Vec3 v1, Vec3 v2 , Vec3 v3, TransformationMatrix* tm, Mesh* mesh)
{
    this->material = material;
    this->objectType = objectType;

    this->mesh = mesh;

    // before setting tm we need to move the center to origin then move back
    this->tm = new TransformationMatrix();
    Vec3 center = (v1 +  v2 + v3 ) / 3.0f;
    // std::cout << "Triangle before transform center: " << center.x << ", " << center.y << ", " << center.z << "\n";

    // if this is not a face then do op. relative to center
    if(this->mesh == nullptr){
        Vec3 center = (v1 +  v2 + v3 ) / 3.0f;
        TransformationMatrix* to_center = new TransformationMatrix(-1*center, 't');
        TransformationMatrix* from_center = new TransformationMatrix(center, 't');
        *(this->tm) = (*from_center) * (*tm) * (*to_center);}
    else
    *(this->tm) =  (*tm);
    

    this->v1 = this->tm->transform(v1);
    this->v2 = this->tm->transform(v2);
    this->v3 = this->tm->transform(v3);
    
    Vec3 new_center = (this->v1 +  this->v2 + this->v3 ) / 3.0f;
    
    // std::cout << "Center: " << new_center.x << ", " << new_center.y << ", " << new_center.z << "\n";


    Vec3 scaled_n = (this->v2-this->v1).cross(this->v3 - this->v1);
    bool det_negative = this->tm->determinant() < 0;
    if(det_negative)
        scaled_n = (this->v3 - this->v1).cross(this->v2-this->v1);

    this->normal = scaled_n.normalize();

    Vec3 minBound;
    Vec3 maxBound;
    minBound.x = std::min({this->v1.x, this->v2.x, this->v3.x});
    minBound.y = std::min({this->v1.y, this->v2.y, this->v3.y});
    minBound.z = std::min({this->v1.z, this->v2.z, this->v3.z});

    // Calculate maximum bounds for x, y, z
    maxBound.x = std::max({this->v1.x, this->v2.x, this->v3.x});
    maxBound.y = std::max({this->v1.y, this->v2.y, this->v3.y});
    maxBound.z = std::max({this->v1.z, this->v2.z, this->v3.z});


    this->min = minBound;
    this->max = maxBound;
    

};

Vec3 Triangle::getBoundingBox(bool isMax){
    if(isMax)
        return this->max;
    return this->min;
}

Vec3  Triangle::getCenter(){
    return (this->v1 + this-> v2 + this->v3 ) / 3.0f;
}

// Function to calculate the area of this->v1 triangle given its vertices
float Triangle::getArea(void) {
    // Vectors AB and AC
    
    Vec3 AB(this->v2.x - this->v1.x, this->v2.y - this->v1.y, this->v2.z - this->v1.z);
    Vec3 AC(this->v3.x - this->v1.x, this->v3.y - this->v1.y, this->v3.z - this->v1.z);

    // Cross product of AB and AC
    Vec3 cross = AB.cross(AC);

    // Area is half the magnitude of the cross product
    return 0.5 * cross.magnitude();
}

Material* Triangle::getMaterial(){
    return this->material;
}
ObjectType Triangle::getObject(){
    return this->objectType;
}

