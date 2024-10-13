
#include "ImagePane.h"
#include "util/util.h"
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
    vector<float> wd = {0,0,0};
    wd[0] = c->w.at(0)* d;
    wd[1] = c->w.at(1)* d;
    wd[2] = c->w.at(2)* d;

    vector<float> m = {0,0,0};
    m[0] = c->getPosition().at(0) - wd.at(0);
    m[1] = c->getPosition().at(1) - wd.at(1);
    m[2] = c->getPosition().at(2) - wd.at(2);
    printf("e:%f%f%f\n", c->getPosition().at(0), c->getPosition().at(1), c->getPosition().at(2));
    printf("w:%f%f%f\n", c->w.at(0), c->w.at(1), c->w.at(2));
    printf("u:%f%f%f\n", c->u.at(0), c->u.at(1), c->u.at(2));
    printf("v:%f%f%f\n", c->v.at(0), c->v.at(1), c->v.at(2));
    printf("Center of image pane is: (%f,%f,%f)\n", m.at(0), m.at(1), m.at(2));

    // center of (0,0) indexed pixels
    vector<float> q = vectorAdd(vectorAdd(m, vectorScale(c->u, l)), vectorScale(c->v, t));

    /*
    
    we can calculate s_ij for each i and j and store them in an array with [i][j] access

    */

   this->sValues = new vector<float>*[dimx];
   for (int i = 0; i < dimx; i++)
   {
    this->sValues[i] = new vector<float>[dimy];
    for (int j = 0; j < dimy; j++)
    {
        float su  = (i+0.5) * (r - l) / dimx;
        float sv = (j+0.5) * (t - b) / dimy;

        vector<float> s = vectorAdd(vectorAdd(q, vectorScale(c->u, su)), vectorScale(c->v, -sv));
        sValues[i][j] = s;
        // printf("S(%f,%f,%f) of i,j: %d,%d\n", s.at(0), s.at(1), s.at(2), i, j);
    }
   }
   

   
};


Ray ImagePane::rayFromCamera(int i, int j){
    // get relevant s and calculate ray
    Ray resultingRay = Ray(
        this->c->getPosition(),
        vectorAdd(this->sValues[i][j], vectorScale(this->c->getPosition(), -1))
    );
    return resultingRay;
}