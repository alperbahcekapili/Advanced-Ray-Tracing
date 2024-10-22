
#include "Triangle.h"
#include "../util/util.h"
#include <math.h>



float Triangle::Intersects(Ray ray){
    vector<float> d = ray.d;
    vector<float> o = ray.o;
    /*
    
    */ 


    float gamma_top[3][3] = {
        {this->v1[0] - this->v2[0], this->v1[0] - o.at(0), d.at(0)},
        {this->v1[1] - this->v2[1], this->v1[1] - o.at(1), d.at(1)},
        {this->v1[2] - this->v2[2], this->v1[2] - o.at(2), d.at(2)}
    };

    float beta_top[3][3] = {
        {this->v1[0] - o.at(0), this->v1[0] - this->v3[0], d.at(0)},
        {this->v1[1] - o.at(1), this->v1[1] - this->v3[1], d.at(1)},
        {this->v1[2] - o.at(2), this->v1[2] - this->v3[2], d.at(2)}
    };

    float t_top[3][3] = {
        {this->v1[0] - this->v2[0], this->v1[0] - this->v3[0], this->v1[0] - o.at(0)},
        {this->v1[1] - this->v2[1], this->v1[1] - this->v3[1], this->v1[1] - o.at(1)},
        {this->v1[2] - this->v2[2], this->v1[2] - this->v3[2], this->v1[2] - o.at(2)}
    };

    float A[3][3] = {
        {this->v1[0] - this->v2[0],this->v1[0] - this->v3[0],d.at(0)},
        {this->v1[1] - this->v2[1],this->v1[1] - this->v3[1],d.at(1)},
        {this->v1[2] - this->v2[2],this->v1[2] - this->v3[2],d.at(2)}
    };

    float beta_ = det3x3(beta_top) / det3x3(A);
    float gamma = det3x3(gamma_top) / det3x3(A);
    float t = det3x3(t_top) / det3x3(A);


    if(beta_ + gamma <= 1 && beta_ >= 0 && gamma >= 0)
        return t;

   return -1;


}

vector<float> Triangle::getSurfaceNormal(vector<float> location){
    return this->normal;
}

Triangle::Triangle(Material* material, ObjectType objectType, float v1[3], float v2[3], float v3[3])
{
    this->material = material;
    this->objectType = objectType;
    this->v1[0] = v1[0];
    this->v1[1] = v1[1];
    this->v1[2] = v1[2];
    this->v2[0] = v2[0];
    this->v2[1] = v2[1];
    this->v2[2] = v2[2];
    this->v3[0] = v3[0];
    this->v3[1] = v3[1];
    this->v3[2] = v3[2];
    // std::cout << this->v1[0];
    // std::cout << this->v1[1];
    // std::cout << this->v1[2];
    vector<float> scaled_n = vectorCrossProduct3D(
        {this->v2[0] - this->v1[0], 
        this->v2[1] - this->v1[1], 
        this->v2[2] - this->v1[2]}, 
    {this->v3[0] - this->v1[0], 
    this->v3[1] - this->v1[1], 
    this->v3[2] - this->v1[2]} );

    std::vector<float> n = normalize(scaled_n);
    this->normal =  std::vector<float>(3,0);
    this->normal.at(0) = n.at(0);
    this->normal.at(1) = n.at(1);
    this->normal.at(2) = n.at(2);
    

};

// Function to calculate the area of this->v1 triangle given its vertices
float Triangle::getArea(void) {
    // Vectors AB and AC
    
    std::vector<float> AB = { this->v2[0] - this->v1[0], this->v2[1] - this->v1[1], this->v2[2] - this->v1[2] };
    std::vector<float> AC = { this->v3[0] - this->v1[0], this->v3[1] - this->v1[1], this->v3[2] - this->v1[2] };

    // Cross product of AB and AC
    std::vector<float> cross = vectorCrossProduct3D(AB, AC);

    // Area is half the magnitude of the cross product
    return 0.5 * getMagnitude(cross);
}

Material* Triangle::getMaterial(){
    return this->material;
}
ObjectType Triangle::getObject(){
    return this->objectType;
}

