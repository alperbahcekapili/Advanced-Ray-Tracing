#include <iostream>
#include <vector>
#include <format>
#include <cstdio>

using namespace std;

#include "Ray.h"

Ray::Ray(const vector<float>& o, const vector<float>& d){
    this->o = o;
    this->d = d;
}
string Ray::toString(){
    char toret[100]; 
    sprintf(toret, "o:(%f,%f,%f}), d:(%f,%f,%f) \n", o.at(0), o.at(1), o.at(2), d.at(0), d.at(1), d.at(2));
    return toret;
}

