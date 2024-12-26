#include "Sphere.h"
#include "../util/util.h"
#include <cmath>  // For sqrt function


TransformationMatrix* Sphere::gettm(){
    return this->tm;
}


Sphere::Sphere(Vec3 center, float R, Material* material, ObjectType objectType, TransformationMatrix* tm, int num_tex_maps, TextureMap* texture_maps)
{
    this->R = R;
    this->center = center;
    this->material = material;
    this->objectType = SphereType;
    this->tex_flags = texture_flags();
    this->texture_maps = texture_maps;
    this->num_tex_maps = num_tex_maps;
    for (size_t i = 0; i < num_tex_maps; i++)
    {
        if (texture_maps[i].decal_mode == replace_kd) {
            this->tex_flags.replace_kd = true;
            this->tex_flags.replace_kd_texture = texture_maps + i;
        } else if (texture_maps[i].decal_mode == blend_kd) {
            this->tex_flags.blend_kd = true;
            this->tex_flags.blend_kd_texture = texture_maps + i;
        } else if (texture_maps[i].decal_mode == replace_ks) {
            this->tex_flags.replace_ks = true;
            this->tex_flags.replace_ks_texture = texture_maps + i;
        } else if (texture_maps[i].decal_mode == bump_normal) {
            this->tex_flags.bump_normal = true;
            this->tex_flags.bump_normal_texture = texture_maps + i;
        } else if (texture_maps[i].decal_mode == replace_all) {
            this->tex_flags.replace_all = true;
            this->tex_flags.replace_all_texture = texture_maps + i;
        } else if (texture_maps[i].decal_mode == replace_normal) {
            this->tex_flags.replace_normal = true;
            this->tex_flags.replace_normal_texture = texture_maps + i;
        } else {
            // Handle any unrecognized decal modes if necessary.
            std::cerr << "Unknown decal mode encountered: " << texture_maps[i].decal_mode << std::endl;
        }

    }
    this->tex_flags.any = (this->tex_flags.replace_kd || this->tex_flags.blend_kd || this->tex_flags.replace_ks || this->tex_flags.bump_normal || this->tex_flags.replace_all || this->tex_flags.replace_normal);

    
    this->tm = new TransformationMatrix();
    
    Vec3 point_on_sphere = Vec3(center.x + R, center.y, center.z);
    TransformationMatrix* to_center = new TransformationMatrix(-1*center, 't');
    TransformationMatrix* from_center = new TransformationMatrix(center, 't');

    // We apply the same transformation to a point on the sphere then calculate new R
    

    *(this->tm) = (*from_center) * (*tm) * (*to_center);
    
    center = this->tm->transform(this->center);
    this->center = center;
    
    Vec3 new_pointon = this->tm->transform(point_on_sphere);
    R = (new_pointon-center).magnitude();
    this->R = R;

    this->min = Vec3(center.x-R, center.y-R, center.z-R);
    this->max = Vec3(center.x+R, center.y+R, center.z+R);

}

Vec3 Sphere::getBoundingBox(bool isMax){
    if(isMax)
        return this->max;
    return this->min;
}
Material* Sphere::getMaterial(){
    return this->material;
}
ObjectType Sphere::getObject(){
    return this->objectType;
}

Vec3  Sphere::getCenter(){
    return this->center;
}


float Sphere::Intersects(Ray ray){
    Vec3 d = ray.d;
    Vec3 o = ray.o;
    Vec3 c = this->center;
    // -d . ( o - c )
    Vec3 ominc =  o - c; 
    float mindoc = -1 * d.dot(ominc);

    // std:cout << "Distance from ray center to sphere center: "<< getMagnitude(ominc) << "\n";

    float dd = d.dot(d);
    float sqrt_el =  pow(mindoc, 2)   -    (dd * (ominc.dot(ominc) - pow(R, 2)));

    // float disc =  4 * pow(dotProduct(d, ominc), 2) - 4 * dd * (dotProduct(ominc, ominc) - pow(R,2));
    // means ray does not intersect with the spehere
    if (sqrt_el < 0){
        // std::cout << "Sphere does not intersect with the ray: " << sqrt_el << "\n" ;    
        return -1.0f;
    }
    float t1 = (((d * -1).dot(o + (c * -1))) + sqrt(sqrt_el)) / dd;
    float t2 = (((d * -1).dot(o + (c * -1))) - sqrt(sqrt_el)) / dd;

    
    // need the smaller one (positive)
    if (t1 > 0 && t1 < t2) {    
        // std::cout << "Sphere hit by ray...\n";
        return t1;
    }
        
    if (t2 > 0 && t2 < t1)
        {
            // std::cout << "Sphere hit by ray...\n";
            return t2;
        }

    if (t1>0 && t2 < 0)
        return t1;
    if (t2> 0 && t1 < 0)
        return t2;
    // std::cout << "Sphere is behind the ray\n";
    
    return -1.0;
}

Vec3 Sphere::getSurfaceNormal(Ray r){

    float mint = this->Intersects(r);
    if(mint < 0)
        return Vec3(-1,-1,-1);
    Vec3 location = r.locationAtT(mint);
    Vec3 scaledVector = (this->center * -1) + location ;
    Vec3 oldnormal =  scaledVector.normalize();

    if( ! this->tex_flags.replace_normal && !this->tex_flags.bump_normal){
        Vec3 scaledVector = (this->center * -1) + location ;
        Vec3 unitVector = scaledVector.normalize();
        return unitVector;
    }



    Vec3 xyz = location - this->center;
    float midlen = sqrt(pow(xyz.x,2) + pow(xyz.z,2));
    float sintheta = midlen / R;
    float cos_phi = (xyz.x/midlen);
    float sinphi = sqrt(1-pow(cos_phi,2));
    Vec3 T = Vec3(2*M_PI*xyz.z, 0, -2*M_PI*xyz.x).normalize();
    Vec3 B = Vec3(M_PI*xyz.y*cos_phi, -R  *M_PI*sintheta, M_PI*xyz.y*sinphi).normalize();
    uv uv_loc = uv::calculateUVSphere(xyz+this->center, this->center, R);
    
    
    if(this->tex_flags.replace_normal){
        Vec3 tex_value = this->tex_flags.replace_normal_texture->interpolateAt(uv_loc , tex_flags.replace_normal_texture->interpolation_type);
        tex_value = (tex_value * 2) - Vec3(1,1,1);
        tex_value = tex_value.normalize(); 
        Vec3 nnew = T*tex_value + B*tex_value + oldnormal*tex_value;
        return nnew.normalize();

    }else{
        Vec3 tex_value = this->tex_flags.bump_normal_texture->interpolateAt(uv_loc , tex_flags.bump_normal_texture->interpolation_type);
        TextureMap* bntex = this->tex_flags.bump_normal_texture;
       
        float du = 1/float(bntex->tim->width);
        float dv = 1/float(bntex->tim->height);

        float u_moved = uv_loc.u + du;
        float v_moved = uv_loc.v + dv;

        uv uv_u_moved = uv() ;
        uv_u_moved.u = u_moved;
        uv_u_moved.v = uv_loc.v;

        uv uv_v_moved = uv();
        uv_v_moved.u = uv_loc.u;
        uv_v_moved.v = v_moved;


        Vec3 dhdu = (bntex->interpolateAt(uv_u_moved, bntex->interpolation_type) - tex_value)/du;
        Vec3 dhdv = (bntex->interpolateAt(uv_v_moved, bntex->interpolation_type) - tex_value)/dv;

        Vec3 dqdu = T + dhdu*oldnormal;
        Vec3 dqdv = B + dhdv*oldnormal;


        Vec3 nnew = dqdu.cross(dqdv);
        return nnew.normalize();

    }







}

Vec3 Sphere::getMotionBlur(){
    return this->motionBlur;
}



int Sphere::get_num_tex_maps(){
    return this->num_tex_maps;
}
TextureMap* Sphere::get_texture_maps() {
    return this->texture_maps;
}
texture_flags Sphere::get_texture_flags(){
    return this->tex_flags;
}