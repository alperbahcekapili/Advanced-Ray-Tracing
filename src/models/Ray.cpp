#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

#include "Ray.h"
#include "../util/util.h"

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

Vec3** Ray::getTangentVectors(){
    float minabsval = this->d.x;
    if(abs(this->d.y) < abs(this->d.x) && abs(this->d.y) < abs(this->d.z))
        minabsval = this->d.y;
    if(abs(this->d.z) < abs(this->d.x) && abs(this->d.z) < abs(this->d.y))
        minabsval = this->d.z;
    
    Vec3 nhat;
    if(minabsval == this->d.x)
        nhat = {1,this->d.y,this->d.z};
    else if(minabsval == this->d.y)    
        nhat = {this->d.x,1,this->d.z};
    else
        nhat = {this->d.x,this->d.y,1};
    

    Vec3* tg1 = new Vec3();
    Vec3* tg2 = new Vec3();

    *tg1 = nhat.cross(this->d);
    *tg2 = this->d.cross(*tg1);

    Vec3** ret_list = new Vec3*[2];
    ret_list[0] = tg1;
    ret_list[1] = tg2;
    return ret_list;
}
