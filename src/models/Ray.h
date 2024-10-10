
#include <iostream>
#include <vector>
using namespace std;


class Ray{
    // Ray is a halflien that is represented by r(t) = o + td with t>=0
    // d and o is three dimensional tuples (2, 1, 3) ex. and t is the magnitude
    public:
    Ray(const vector<int>& o, const vector<int>& d, int t);
    string toString();
    bool intersectRay(Ray other);
    
    private:
    vector<int> o;
    vector<int> d;
    int t;

};