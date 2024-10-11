
#include "ImagePane.h"

ImagePane::ImagePane(int dimx, int dimy, int l, int r, int b, int t, int d, Camera c)
{
    this->dimx = dimx;
    this->dimy = dimy;
    this->l = l;
    this->r = r;
    this->b = b;
    this->t = t;
    this->d = d;


    // We need to intialize the image pane with Camera thus we can precompute all ray functions for each pixel
    
};