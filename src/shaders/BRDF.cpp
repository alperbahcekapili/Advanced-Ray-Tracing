#include "BRDF.h"
#include <math.h>

BRDF::BRDF(BRDFType brdf_type){
    this->brdf_type = brdf_type;
}

BRDF::~BRDF(){
    
}


Vec3 BRDF::originalBlinnPhong(brdf_inputs inputs){
    Vec3 wh = (inputs.wi + inputs.wo).normalize();
    float cosTheta = inputs.n.dot(inputs.wi);
    float cosAlpha = inputs.n.dot(wh);
    Vec3 f = inputs.kd + (inputs.ks * (pow(cosAlpha, this->phong_exponent) / cosTheta));
    if (cosTheta < 0)
        return Vec3(0,0,0);
    return f; // 90 degree check
}

Vec3 BRDF::originalPhong(brdf_inputs inputs){
    Vec3 perfect_reflection = (inputs.wi - 2 * (inputs.wi.dot(inputs.n)) * inputs.n).normalize();
    float cosAlpha = perfect_reflection.dot(inputs.wo);    
    float cosTheta = inputs.n.dot(inputs.wi);
    Vec3 f = inputs.kd + (inputs.ks * (pow(cosAlpha, this->phong_exponent) / cosTheta));
    if (cosTheta < 0)
        return Vec3(0,0,0);
    return f; // 90 degree check
}

Vec3 BRDF::modifiedPhong(brdf_inputs inputs){
    Vec3 perfect_reflection = (inputs.wi - 2 * (inputs.wi.dot(inputs.n)) * inputs.n).normalize();
    float cosAlpha = perfect_reflection.dot(inputs.wo);    
    float cosTheta = inputs.n.dot(inputs.wi);
    Vec3 f = inputs.kd + (inputs.ks * pow(cosAlpha, this->phong_exponent));
    if (cosTheta < 0)
        return Vec3(0,0,0);
    return f; // 90 degree check
}

Vec3 BRDF::modifiedBlinnPhong(brdf_inputs inputs){
    return 0.;
}

Vec3 BRDF::torranceSparrow(brdf_inputs inputs){
    return 0.;
}

Vec3 BRDF::f(brdf_inputs inputs){
    switch (this->brdf_type)
    {
        case OriginalBlinnPhong:
            return this->originalBlinnPhong(inputs);
            break;
        case OriginalPhong:
            return this->originalPhong(inputs);
            break;
        case ModifiedBlinnPhong:
            return this->modifiedBlinnPhong(inputs);
            break;
        case ModifiedPhong:
            return this->modifiedPhong(inputs);
            break;
        case TorranceSparrow:
            return this->torranceSparrow(inputs);
            break;

    }
}