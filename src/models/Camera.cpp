#include "Camera.h"
#include "../util/util.h"
#include <vector>
#include <iostream>


Camera::Camera(Vec3  v, Vec3  w, Vec3  position, float mint, float maxt, string name):mint(mint), maxt(maxt)
{
this->v = v;
// calculate and store w
this->w = w;
this->u = w.cross(v);
this->position = position;
std::cout << position.x << ", " << position.y << ", " << position.z << "\n";
this->name = name;
}

Vec3  Camera::getPosition(){
    return this->position;
}
