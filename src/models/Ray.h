#pragma once

#include <iostream>
#include <vector>
using namespace std;
#include "../util/data_structures.h"

class Ray{
    // Ray is a halflien that is represented by r(t) = o + td with t>=0
    // d and o is three dimensional tuples (2, 1, 3) ex. and t is the magnitude
    public:
    Ray(Vec3 o, Vec3 d);
    string toString();
    bool intersectRay(Ray other);
    Vec3 locationAtT(float t);
    Vec3 o;
    Vec3 d;
    Vec3** getTangentVectors();
    float time;

    private:

};