#pragma once
#include "TextureImage.h"
#include "../models/Ray.h"


enum DecalMode{
    replace_kd, 
    blend_kd,
    replace_ks,
    replace_background,
    bump_normal,
    replace_all
};

struct uv{
    float u;
    float v;


    static uv calculateUVTriangle(
    const Vec3& p, // Intersection point
    const Vec3& a, const Vec3& b, const Vec3& c, // Triangle vertices
    const std::pair<float, float>& uv_a, // Texture coordinates at vertex a
    const std::pair<float, float>& uv_b, // Texture coordinates at vertex b
    const std::pair<float, float>& uv_c  // Texture coordinates at vertex c,
) {
    // Compute the triangle edges
    Vec3 v0 = b - a;
    Vec3 v1 = c - a;
    Vec3 v2 = p - a;

    // Compute dot products
    float d00 = v0.dot(v0);
    float d01 = v0.dot(v1);
    float d11 = v1.dot(v1);
    float d20 = v2.dot(v0);
    float d21 = v2.dot(v1);

    // Compute denominator
    float denom = d00 * d11 - d01 * d01;

    // Compute barycentric coordinates
    float beta = (d20 * d11 - d21 * d01) / denom;
    float gamma = (d21 * d00 - d20 * d01) / denom;
    float alpha = 1.0 - beta - gamma;

    // Interpolate texture coordinates
    float u = alpha * uv_a.first + beta * uv_b.first + gamma * uv_c.first;
    float v = alpha * uv_a.second + beta * uv_b.second + gamma * uv_c.second;
    uv tmp_uv;
    tmp_uv.u = u;
    tmp_uv.v = v;
    return tmp_uv;
};



static uv calculateUVSphere(
    const Vec3& p,         // Point on the sphere surface
    const Vec3& center,    // Center of the sphere
    float radius          // Radius of the sphere
) {
    // Translate the point to the sphere's local coordinate system
    Vec3 local_p = p - center;

    // Normalize the local point to ensure it lies on the sphere surface
    Vec3 normalized_p = local_p / radius;

    // Extract the x, y, z coordinates
    float x = normalized_p.x;
    float y = normalized_p.y;
    float z = normalized_p.z;

    // Calculate spherical coordinates
    float theta = std::acos(y);          // Polar angle (Θ)
    float phi = std::atan2(z, x);        // Azimuthal angle (ϕ)

    // Convert spherical coordinates to UV
    float u = (-phi + M_PI) / (2 * M_PI); // Map φ to [0, 1]
    float v = theta / M_PI;               // Map Θ to [0, 1]

    uv tmp_uv ;
    tmp_uv.u = u;
    tmp_uv.v = v;
    return tmp_uv;
};




};

class TextureMap {
    public:
    TextureImage* tim;
    DecalMode decal_mode; 
    InterploationType interpolation_type;
    bool is_image;
    TextureMap();
    TextureMap(TextureImage* texture_image, bool is_image, DecalMode decal_mode, InterploationType interpolation_type);
    static DecalMode getDecalMode(const char* mode);
    ~TextureMap();
    Vec3 interpolateAt(uv loc, InterploationType interploation_type);
    TextureImage* getTim();
    static InterploationType getInterpolationType(const char* mode);
};