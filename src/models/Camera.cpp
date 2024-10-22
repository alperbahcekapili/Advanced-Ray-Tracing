#include "Camera.h"
#include "../util/util.h"
#include <vector>
#include <iostream>


Camera::Camera(std::vector<float> v, std::vector<float> w, std::vector<float> position, float mint, float maxt, string name):mint(mint), maxt(maxt)
{
this->v = v;
// calculate and store w
this->w = w;
this->u = vectorCrossProduct3D(w,v);
std::cout << "Agaaa bak w: \n" << w.at(0) << "," << w.at(1) << "," << w.at(2) << "\n" ;
this->position = position;
this->name = name;
}

std::vector<float> Camera::getPosition(){
    return this->position;
}
