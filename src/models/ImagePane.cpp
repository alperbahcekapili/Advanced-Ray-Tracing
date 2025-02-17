
#include "ImagePane.h"
#include "../util/util.h"
#include <stdlib.h>
#include <random>



ImagePane::ImagePane(int dimy, int dimx, float l, float r, float b, float t, float d, Camera* c)
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
    
    printf("e:%f%f%f\n", c->getPosition().x, c->getPosition().y, c->getPosition().z);
    printf("w:%f%f%f\n", c->w.x, c->w.y, c->w.z);
    printf("u:%f%f%f\n", c->u.x, c->u.y, c->u.z);
    printf("v:%f%f%f\n", c->v.x, c->v.y, c->v.z);
    printf("Center of image pane is: (%f,%f,%f)\n", m.x, m.y, m.z);
    printf("xdim:%d, ydim:%d\n", dimx, dimy);

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
        // std::cout << "i: " << i << ", j:" << j << ", r: " << r << ", l " << ", l " << l << "\n";
        float su = (float(i) + 0.5f) * (float(r) - float(l)) / float(dimx);
        float sv = (float(j) + 0.5f) * (float(t) - float(b)) / float(dimy);

        // std::cout << su  <<  "," << sv  << "\n";
        Vec3  s = (q + (c->u * su)) + (c->v * -sv);
        sValues[i][j] = s;
        // printf("S(%f,%f,%f) of i,j: %d,%d\n", s.x, s.y, s.z, i, j);
    }
   }
   

   
};


Ray ImagePane::rayFromCamera(int i, int j, int rayindex){
    // i and j already specify jitter
    // 
    float offsetx = generate_random_01();
    float offsety = generate_random_01();
    

    int num_sample_axis =  int(sqrt(this->c->numsamples));
    float subregion_width = (r - l)  / (float(num_sample_axis)* dimx);
    float subregion_height = (t - b) / (float(num_sample_axis)* dimy);
    Vec3 pixel_center = this->sValues[i][j];
    
    int xgrid = int(rayindex%num_sample_axis);
    int ygrid = int(rayindex/num_sample_axis);
    
    float pixel_start_x = pixel_center.x + subregion_width*xgrid;
    float pixel_start_y = pixel_center.y + subregion_height*ygrid;

    // first go to relevant subgrid then add random offset
    float samplex = pixel_start_x + offsetx*subregion_width;
    float sampley = pixel_start_y + offsety*subregion_width;

    // below is pixel sample
    Vec3 sampled_pos = Vec3(samplex, sampley, pixel_center.z);
    Vec3 direction = (sampled_pos - this->c->getPosition()).normalize();
    // need to generate below for aperture
    if(this->c->aperture_size!= -1){

        float ap_offsetx = (generate_random_01()-0.5)*this->c->aperture_size;
        float ap_offsety = (generate_random_01()-0.5)*this->c->aperture_size;

        // aperture center is assumed to be on camera center
        Vec3 campos = this->c->getPosition();
        Vec3 a(campos.x + ap_offsetx, campos.y + ap_offsety, campos.z);
        Vec3 dir = (sampled_pos - campos).normalize();
        float t = this->c->focus_distance/dir.dot(this->c->w);// because I use w as gaze I do not *-1
        Vec3 p = campos + (dir*t);
        Vec3 d = (p - a).normalize();
        direction = d;
        sampled_pos = a;
    }


    Ray resultingRay = Ray(
        sampled_pos,
        direction
    );

    // resultingRay.time = generate_random_01();

    return resultingRay;
}