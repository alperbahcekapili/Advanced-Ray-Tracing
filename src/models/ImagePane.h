#pragma once
#include "Ray.h"
#include "Camera.h"

class ImagePane
{
private:
    float l;
    float r;
    float b;
    float t;
    float d;
    Vec3** sValues;
    Camera* c;
public:
    int dimx;
    int dimy;
    ImagePane(int dimx, int dimy, float l, float r, float b, float t, float d, Camera* c);
    Ray rayFromCamera(int i, int j);
};

