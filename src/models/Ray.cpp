#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

#include "Ray.h"
#include "../util/util.h"

Ray::Ray(Vec3 o, Vec3 d){
    this->o = o;
    this->d = d.normalize();
}

Vec3 Ray::locationAtT(float t){
    return o + (d * t);
}

string Ray::toString(){
    char toret[100]; 
    printf(toret, "o:(%f,%f,%f}), d:(%f,%f,%f) \n", o.x, o.y, o.z, d.x, d.y, d.z);
    return toret;
}

