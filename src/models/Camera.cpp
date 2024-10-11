#include "Camera.h"
#include "util/util.cpp"
#include <vector>
#include <iostream>


Camera::Camera(std::vector<float> u, std::vector<float> v, std::vector<float> position)
{
this->u = u;
this->v = v;
// calculate and store w
this->w = vectorCrossProduct3D(u,v);

std::cout << "Agaaa bak w: \n" << w.at(0) << "," << w.at(1) << "," << w.at(2) << "\n" ;
this->position = position;
}

std::vector<float> Camera::getPosition(){
    return this->position;
}
