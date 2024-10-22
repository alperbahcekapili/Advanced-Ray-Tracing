#pragma once
#include "Ray.h"
#include "Camera.h"

class ImagePane
{
private:
    int l;
    int r;
    int b;
    int t;
    float d;
    vector<float>** sValues;
    Camera* c;
public:
    int dimx;
    int dimy;
    ImagePane(int dimx, int dimy, int l, int r, int b, int t, float d, Camera* c);
    Ray rayFromCamera(int i, int j);
};

