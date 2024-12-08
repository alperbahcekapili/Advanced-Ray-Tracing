#include "Camera.h"
#include "../util/util.h"
#include <vector>
#include <iostream>


Camera::Camera(Vec3  v, Vec3  w, Vec3  position, float mint, float maxt, string name, float numsamples, float focus_distance, float aperture_size):mint(mint), maxt(maxt)
{
this->v = v.normalize();
this->w = w.normalize();
this->u = w.cross(v).normalize();
this->position = position;
// std::cout << position.x << ", " << position.y << ", " << position.z << "\n";
this->name = name;
this->numsamples = numsamples;
this->focus_distance = focus_distance;
this->aperture_size = aperture_size;
}

Vec3  Camera::getPosition(){
    return this->position;
}
