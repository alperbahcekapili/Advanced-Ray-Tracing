#include "Camera.h"
#include "../util/util.h"
#include <vector>
#include <iostream>


Camera::Camera(std::vector<float> u, std::vector<float> v, std::vector<float> position, float mint, float maxt, string name):mint(mint), maxt(maxt)
{
this->u = u;
this->v = v;
// calculate and store w
this->w = vectorCrossProduct3D(u,v);

std::cout << "Agaaa bak w: \n" << w.at(0) << "," << w.at(1) << "," << w.at(2) << "\n" ;
this->position = position;
this->name = name;
}

std::vector<float> Camera::getPosition(){
    return this->position;
}
