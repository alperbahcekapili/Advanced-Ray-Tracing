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