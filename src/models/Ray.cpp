#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

#include "Ray.h"
#include "util/util.h"

Ray::Ray(const vector<float>& o, const vector<float>& d){
    this->o = o;
    this->d = d;
}

vector<float> Ray::locationAtT(float t){
    return vectorAdd(o, vectorScale(d, t));
}

string Ray::toString(){
    char toret[100]; 
    printf(toret, "o:(%f,%f,%f}), d:(%f,%f,%f) \n", o.at(0), o.at(1), o.at(2), d.at(0), d.at(1), d.at(2));
    return toret;
}

