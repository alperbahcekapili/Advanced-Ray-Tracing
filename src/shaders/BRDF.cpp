#include "BRDF.h"
#include <math.h>

BRDF::BRDF(BRDFType brdf_type){
    this->brdf_type = brdf_type;
}

BRDF::~BRDF(){
    
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


Vec3 BRDF::normalizedModifiedPhong(brdf_inputs inputs){
    float n_factor = 50; // TODO replace this with the actual value

    Vec3 perfect_reflection = (inputs.wi - 2 * (inputs.wi.dot(inputs.n)) * inputs.n).normalize();
    Vec3 wh = (inputs.wi + inputs.wo).normalize();
    float cosTheta = inputs.n.dot(inputs.wi);
    float cosAlpha = perfect_reflection.dot(inputs.wo);    
    Vec3 f = (inputs.kd/M_PI) + (inputs.ks * ((this->phong_exponent + 2)/(2*M_PI)) * pow(cosAlpha, n_factor));
    if (cosTheta < 0)
        return Vec3(0,0,0);
    return f; // 90 degree check
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



Vec3 BRDF::modifiedBlinnPhong(brdf_inputs inputs){
    Vec3 wh = (inputs.wi + inputs.wo).normalize();
    float cosTheta = inputs.n.dot(inputs.wi);
    float cosAlpha = inputs.n.dot(wh);
    Vec3 f = inputs.kd + (inputs.ks * pow(cosAlpha, this->phong_exponent) );
    if (cosTheta < 0)
        return Vec3(0,0,0);
    return f; // 90 degree check
}


Vec3 BRDF::normalizedModifiedBlinnPhong(brdf_inputs inputs){
    float n_factor = 50; // todo replace with the actual value
    Vec3 wh = (inputs.wi + inputs.wo).normalize();
    float cosTheta = inputs.n.dot(inputs.wi);
    float cosAlpha = inputs.n.dot(wh);
    Vec3 f = (inputs.kd/M_PI) + (inputs.ks* ((this->phong_exponent + 8)/(8*M_PI)) * pow(cosAlpha, n_factor) );
    if (cosTheta < 0)
        return Vec3(0,0,0);
    return f; // 90 degree check
}

Vec3 BRDF::torranceSparrow(brdf_inputs inputs){
    Vec3 wh = (inputs.wi + inputs.wo).normalize();
    float cosAlpha = wh.dot(inputs.n);
    float Dalpha = ((this->phong_exponent + 2) / (2*M_PI)) * pow(cosAlpha, this->phong_exponent);
    
    // Calculate G
    float nwh = inputs.n.dot(wh);
    float nwi = inputs.n.dot(inputs.wi);
    float nwo = inputs.n.dot(inputs.wo);
    float wowh = inputs.wo.dot(wh);
    float G = fmin(1, fmin((2*nwh*nwo)/wowh, (2*nwh*nwi)/wowh));
    float R0 = pow(inputs.refraction_index - 1, 2) / pow(inputs.refraction_index + 1, 2);
    float cos_beta = wowh;
    float F = R0 + (1 - R0) * pow(1 - cos_beta, 5);
    if (this->kdfresnel)
        ((1-F) * (inputs.kd / M_PI)) + (inputs.ks * Dalpha * F * G / (4 * nwi * nwo));
    return inputs.kd / M_PI + (inputs.ks * Dalpha * F * G / (4 * nwi * nwo));
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
        case NormalizedModifiedPhong:
            return this->normalizedModifiedPhong(inputs);
            break;
        case NormalizedModifiedBlinnPhong:
            return this->normalizedModifiedBlinnPhong(inputs);
            break;

    }
}