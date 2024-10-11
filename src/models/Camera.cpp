#include "Camera.h"
#include <vector>
#include <iostream>


Camera::Camera(std::vector<int> u, std::vector<int> v, std::vector<int> poisition)
{
this->u = u;
this->v = v;
// calculate and store w
float wx = u.at(1)* v.at(2) - u.at(2) * v.at(1);
float wy = - u.at(0)* v.at(2) + u.at(2)* v.at(0);
float wz = u.at(0)* v.at(1) - u.at(1)* v.at(0);
this->w.resize(3);
this->w[0] = wx;
this->w[1] = wy;
this->w[2] = wz;

std::cout << wx << "," << wy << "," << wz << "\n" ;
this->position = position;
}

std::vector<int> Camera::getPosition(){
    return this->position;
}
