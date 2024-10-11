#pragma once

#include <iostream>
#include <vector>
using namespace std;


class Ray{
    // Ray is a halflien that is represented by r(t) = o + td with t>=0
    // d and o is three dimensional tuples (2, 1, 3) ex. and t is the magnitude
    public:
    Ray(const vector<float>& o, const vector<float>& d);
    string toString();
    bool intersectRay(Ray other);
    
    vector<float> o;
    vector<float> d;
    
    private:

};