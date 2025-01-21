

#include <vector>
#include <cassert>
#include "../models/Ray.h"
#include "../models/Object.h"
#include "../models/Ray.h"
#include "../lights/Light.h"
#include "../lights/DirectionalLight.h"
#include "../lights/SphericalDirectionalLight.h"
#include "util.h"
#include "tinyexr.h"
#include <cstring>
#include <math.h>

using namespace std;

#define assertm(exp, msg) assert(((void)msg, exp))
#include <algorithm>
#include <limits>

void scale_floats_0_1(float* data, size_t size) {
    // Find the minimum and maximum values in the array
    float min_val = *std::min_element(data, data + size);
    float max_val = *std::max_element(data, data + size);

    // Handle the case where all values are the same
    if (min_val == max_val) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = 0.0f;
        }
        return;
    }

    // Scale the values to the range [0, 1]
    for (size_t i = 0; i < size; ++i) {
        data[i] = (data[i] - min_val) / (max_val - min_val);
    }
}




void convertVec3ToFlatArray(Vec3** image, int width, int height, float* flatArray) {
    int index = 0;
    for (int j = 0; j < height ; ++j)
    {
        for (int i = 0; i < width; ++i)
        
        {
            Vec3 pixel = image[i][j];
            // printf("%f ", pixel.x);
            // printf("%f ", pixel.y);
            // printf("%f \n", pixel.z);
            flatArray[index++] = std::min(pixel.x, 255.0f);
            flatArray[index++] = std::min(pixel.y, 255.0f);
            flatArray[index++] = std::min(pixel.z, 255.0f);
        }
    }
}







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
    if(light->ltype == DirectionalLightType){
        DirectionalLight* dir_light = dynamic_cast<DirectionalLight*>(light);
        Vec3 dir = dir_light->getDirection();
        Vec3 origin = destination - (99999 * dir); // light comes from out of the scene
        return Ray(origin, dir);
    }
    else if (light->ltype == SphericalDirectionalLightType)
    {
        SphericalDirectionalLight* sph_light = dynamic_cast<SphericalDirectionalLight*>(light);
        Vec3 randomdir  = Vec3(generate_random_01(), generate_random_01(), generate_random_01());
        return Ray(Vec3(0,0,0), randomdir);
    }
    
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