

#include <vector>
#include <cassert>
#include "../models/Ray.h"
#include "../models/Object.h"
#include "../lights/Light.h"
#include "util.h"
#include <math.h>

using namespace std;

#define assertm(exp, msg) assert(((void)msg, exp))



float det3x3(float m[3][3]){
    // satir, sutun
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[1][0] * (m[0][2] * m[2][1] - m[0][1] * m[2][2]) + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
}

Vec3 clipValues(Vec3 v, float clip_val){
    if(v.x > clip_val)
        v.x  = clip_val;
    if(v.y  > clip_val)
        v.y  = clip_val;
    if(v.z > clip_val)
        v.z = clip_val;
    return v;
    
}


Ray createRayFrom(Vec3 start, Vec3 destination){
    // In order to create ray from start and destination we need to substract start from destination
    Vec3 scaledRay = destination - start;
    return Ray(start, scaledRay);
}


Ray createLightRay(Light* light, Vec3 destination){
    Vec3 start = light->getPointOn();
    return createRayFrom(start, destination);
}


void printObjectList(Object** l, int len){
    for (size_t i = 0; i < len; i++)
    {
        std::cout << i << ": " <<l[i]->getCenter().x << ", " << l[i]->getCenter().y << ", " << l[i]->getCenter().z << "\n";
    }
    std::cout << "\n\n";
    
}