
#include "Triangle.h"
#include "../util/util.h"
#include "../util/data_structures.h"
#include <math.h>



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

Vec3 Triangle::getSurfaceNormal(Vec3 location){
    return this->normal;
}

Triangle::Triangle(){

}

Triangle::Triangle(Material* material, ObjectType objectType, Vec3 v1, Vec3 v2 , Vec3 v3)
{
    this->material = material;
    this->objectType = objectType;
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    

    Vec3 center = Vec3::centroid(v1, v2, v3);
    Vec3 normal = (v2 - v1).cross(v3 - v1);  // Calculate the normal for orientation

    // Sort vertices based on signed angle with respect to the centroid and normal
    std::array<Vec3, 3> vertices = {v1, v2, v3};
    std::sort(vertices.begin(), vertices.end(), [&](const Vec3& a, const Vec3& b) {
        return Vec3::signedAngle(center, a, normal) < Vec3::signedAngle(center, b, normal);
    });

    // Update v1, v2, v3 with sorted vertices
    v1 = vertices[0];
    v2 = vertices[1];
    v3 = vertices[2];


    Vec3 scaled_n = (this->v2-this->v1).cross(this->v3 - this->v1);
    this->normal = scaled_n.normalize();
    
    

};

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

