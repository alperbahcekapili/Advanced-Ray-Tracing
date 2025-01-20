#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

#include "Ray.h"
#include "../util/util.h"


Ray::Ray(){
    this->o = Vec3(0,0,0);
    this->d = Vec3(0,0,0);
    this->time = -1;
}
Ray::Ray(Vec3 o, Vec3 d){
    this->o = o;
    this->d = d.normalize();
    this->time = -1;
}

Vec3 Ray::locationAtT(float t){
    return o + (d * t);
}

string Ray::toString(){
    char toret[100]; 
    printf(toret, "o:(%f,%f,%f}), d:(%f,%f,%f) \n", o.x, o.y, o.z, d.x, d.y, d.z);
    return toret;
}
Vec3** Ray::getTangentVectors() {
    // Ensure this->d is normalized
    Vec3 d_normalized = this->d.normalize();

    // Select a component with the smallest absolute value
    Vec3 nhat;
    if (std::abs(d_normalized.x) <= std::abs(d_normalized.y) && std::abs(d_normalized.x) <= std::abs(d_normalized.z)) {
        nhat = Vec3(1.0f, 0.0f, 0.0f);
    } else if (std::abs(d_normalized.y) <= std::abs(d_normalized.x) && std::abs(d_normalized.y) <= std::abs(d_normalized.z)) {
        nhat = Vec3(0.0f, 1.0f, 0.0f);
    } else {
        nhat = Vec3(0.0f, 0.0f, 1.0f);
    }

    // Calculate tangent vectors
    Vec3* tg1 = new Vec3();
    Vec3* tg2 = new Vec3();

    *tg1 = nhat.cross(d_normalized).normalize(); // First tangent
    *tg2 = d_normalized.cross(*tg1).normalize(); // Second tangent

    // Allocate and return results
    Vec3** ret_list = new Vec3*[2];
    ret_list[0] = tg1;
    ret_list[1] = tg2;
    return ret_list;
}