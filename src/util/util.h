#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#include <vector>
#include <string>
#include <cassert>
#include "../models/Ray.h"
#include "data_structures.h"
#include "../lights/Light.h"


Ray createRayFrom(Vec3 start, Vec3 destination);
Ray createLightRay(Light* light, Vec3 destination);
Vec3 clipValues(Vec3 v, float clip_val);

float det3x3(float m[3][3]);

#endif // VECTOR_UTILS_H


#include "../models/Object.h"
void printObjectList(Object** l, int len);
void convertVec3ToFlatArray(Vec3** image, int width, int height, float* flatArray);
void reinhardGlobalTonemap(Vec3** image, int width, int height, float burnout, float saturation, float alpha = 0.18 );
void degammaCorrectImage(Vec3** image, int width, int height, float gamma);
Vec3 degammaCorrectColor(const Vec3& color, float gamma) ;
Vec3 gammaCorrectColor(const Vec3& color, float gamma);
void gammaCorrectImage(Vec3** image, int width, int height, float gamma) ;
void degammaCorrectImage(float* image, int channels, int width, int height, float gamma);

float degammaCorrectColor(float& color, float gamma);
float gammaCorrectColor(float& color, float gamma);
void gammaCorrectImage(float* image, int channels, int width, int height, float gamma);
bool SaveEXR(const float* rgb, int width, int height, const char* outfilename);
void scale_floats_0_1(float* data, size_t size);