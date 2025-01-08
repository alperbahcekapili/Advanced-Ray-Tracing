#pragma once

#include "../util/data_structures.h"

struct brdf_inputs{
    Vec3 wi;
    Vec3 wo;
    Vec3 n;
    Vec3 kd;
    Vec3 ks;
    float refraction_index;
    
    
};

enum BRDFType{
    OriginalBlinnPhong,
    OriginalPhong,
    ModifiedBlinnPhong,
    ModifiedPhong,
    TorranceSparrow,
    NormalizedModifiedPhong,
    NormalizedModifiedBlinnPhong

};
class BRDF{
    public:

        float phong_exponent;
        bool kdfresnel = false;

        BRDFType brdf_type;
        Vec3  f(brdf_inputs inputs);
        BRDF(BRDFType brdf_type);
        Vec3 originalBlinnPhong(brdf_inputs inputs);
        Vec3 originalPhong(brdf_inputs inputs);
        Vec3 modifiedBlinnPhong(brdf_inputs inputs);
        Vec3 modifiedPhong(brdf_inputs inputs);
        Vec3 torranceSparrow(brdf_inputs inputs);
        Vec3 normalizedModifiedPhong(brdf_inputs inputs);
        Vec3 normalizedModifiedBlinnPhong(brdf_inputs inputs);
    
        ~BRDF();
};