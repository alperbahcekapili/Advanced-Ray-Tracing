#include "Ray.h"
#include "Camera.h"

class ImagePane
{
private:
    int dimx;
    int dimy;
    int l;
    int r;
    int b;
    int t;
    int d;
public:
    ImagePane(int dimx, int dimy, int l, int r, int b, int t, int d, Camera c);
    Ray rayFromCamera(int i, int j);
};

