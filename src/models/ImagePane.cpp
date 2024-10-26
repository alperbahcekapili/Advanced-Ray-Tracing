
#include "ImagePane.h"
#include "../util/util.h"
#include <stdlib.h>
#include <random>



ImagePane::ImagePane(int dimx, int dimy, int l, int r, int b, int t, float d, Camera* c)
{
    this->dimx = dimx;
    this->dimy = dimy;
    this->l = l;
    this->r = r;
    this->b = b;
    this->t = t;
    this->d = d;
    this->c = c;

    // We need to intialize the image pane with Camera thus we can precompute all ray functions for each pixel
    Vec3  wd(c->w.x * d, c->w.y * d, c->w.z * d);
    Vec3  m (c->getPosition().x + wd.x, c->getPosition().y + wd.y, c->getPosition().z + wd.z);
    
    // printf("e:%f%f%f\n", c->getPosition().at(0), c->getPosition().at(1), c->getPosition().at(2));
    // printf("w:%f%f%f\n", c->w.at(0), c->w.at(1), c->w.at(2));
    // printf("u:%f%f%f\n", c->u.at(0), c->u.at(1), c->u.at(2));
    // printf("v:%f%f%f\n", c->v.at(0), c->v.at(1), c->v.at(2));
    // printf("Center of image pane is: (%f,%f,%f)\n", m.at(0), m.at(1), m.at(2));

    // center of (0,0) indexed pixels
    Vec3  q = (m + (c->u * l)) + (c->v * t);

    /*
    
    we can calculate s_ij for each i and j and store them in an array with [i][j] access

    */

   this->sValues = new Vec3*[dimx];
   for (int i = 0; i < dimx; i++)
   {
    this->sValues[i] = new Vec3[dimy];
    for (int j = 0; j < dimy; j++)
    {
        float su  = (i+0.5) * (r - l) / dimx;
        float sv = (j+0.5) * (t - b) / dimy;

        Vec3  s = (q + (c->u * su)) + (c->v * -sv);
        sValues[i][j] = s;
        // printf("S(%f,%f,%f) of i,j: %d,%d\n", s.at(0), s.at(1), s.at(2), i, j);
    }
   }
   

   
};


Ray ImagePane::rayFromCamera(int i, int j){
    this->c->getPosition();
    Ray resultingRay = Ray(
        this->sValues[i][j],
        this->sValues[i][j] + (this->c->getPosition() * -1)
    );
    return resultingRay;
}