#include "Object.h"
class Triangle : Object
{
private:
    double v1[3];
    double v2[3];
    double v3[3];
    Material* material;
    ObjectType objectType;

    static double magnitude(const std::array<double, 3>& vec) ;
    static std::array<double, 3> crossProduct(const std::array<double, 3>& vec1, const std::array<double, 3>& vec2);


public:
    Triangle(Material* material, ObjectType objectType, double v1[3], double v2[3], double v3[3]);
    double getArea(void);
};
