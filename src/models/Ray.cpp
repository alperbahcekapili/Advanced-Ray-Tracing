#include <iostream>
#include <vector>
#include <format>
#include <cstdio>

using namespace std;

#include "Ray.h"

Ray::Ray(const vector<int>& o, const vector<int>& d, int t){
    this->o = o;
    this->d = d;
    this->t = t;
}
string Ray::toString(){
    char toret[100]; 
    sprintf(toret, "o:(%d,%d,%d}), d:(%d,%d,%d), t:%d \n", o.at(0), o.at(1), o.at(2), d.at(0), d.at(1), d.at(2), t);
    return toret;
}

