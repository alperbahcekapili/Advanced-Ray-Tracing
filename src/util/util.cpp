

#include <vector>
#include <cassert>
#include "../models/Ray.h"
#include "../models/Object.h"
#include "../models/Ray.h"
#include "../lights/Light.h"
#include "../lights/DirectionalLight.h"
#include "util.h"

#include <math.h>

using namespace std;

#define assertm(exp, msg) assert(((void)msg, exp))





void reinhardGlobalTonemap(Vec3** image, int width, int height, float burnout, float alpha = 0.18 ) {
    const float epsilon = 1e-6;

    // Step 1: Compute luminance
    std::vector<float> luminanceValues;
    luminanceValues.reserve(width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Vec3 pixel = image[y][x];
            float luminance = 0.2126f * pixel.x + 0.7152f * pixel.y + 0.0722f * pixel.z;
            luminanceValues.push_back(luminance);
        }
    }

    // Step 2: Compute the logarithmic average luminance
    float logSum = 0.0f;
    for (float luminance : luminanceValues) {
        logSum += std::log(luminance + epsilon);
    }
    float logAvgLuminance = std::exp(logSum / luminanceValues.size());

    // Step 3: Scale the luminance
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Vec3& pixel = image[y][x];
            float luminance = 0.2126f * pixel.x + 0.7152f * pixel.y + 0.0722f * pixel.z;
            float scaledLuminance = (alpha / logAvgLuminance) * luminance;

            // Step 4: Apply tone mapping with burnout parameter
            float mappedLuminance = (scaledLuminance * (1.0f + scaledLuminance / (burnout * burnout))) / (1.0f + scaledLuminance);

            // Step 5: Reapply colors
            if (luminance > epsilon) {
                float scale = mappedLuminance / luminance;
                pixel.x *= scale;
                pixel.y *= scale;
                pixel.z *= scale;
            }
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