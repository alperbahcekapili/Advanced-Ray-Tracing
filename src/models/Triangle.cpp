
#include "Triangle.h"
#include "../util/util.h"
#include "../util/data_structures.h"
#include <math.h>


TransformationMatrix* Triangle::gettm(){
    return this->tm;
}
float Triangle::Intersects(Ray ray){
    Vec3 d = ray.d;
    Vec3 o = ray.o;
    

    Vec3 v1 = this->tm->transform(this->v1);
    Vec3 v2 = this->tm->transform(this->v2);
    Vec3 v3 = this->tm->transform(this->v3);

    float gamma_top[3][3] = {
        {v1.x - v2.x, v1.x - o.x, d.x},
        {v1.y - v2.y, v1.y - o.y, d.y},
        {v1.z - v2.z, v1.z - o.z, d.z}
    };

    float beta_top[3][3] = {
        {v1.x - o.x, v1.x - v3.x, d.x},
        {v1.y - o.y, v1.y - v3.y, d.y},
        {v1.z - o.z, v1.z - v3.z, d.z}
    };

    float t_top[3][3] = {
        {v1.x - v2.x, v1.x - v3.x, v1.x - o.x},
        {v1.y - v2.y, v1.y - v3.y, v1.y - o.y},
        {v1.z - v2.z, v1.z - v3.z, v1.z - o.z}
    };

    float A[3][3] = {
        {v1.x - v2.x,v1.x - v3.x,d.x},
        {v1.y - v2.y,v1.y - v3.y,d.y},
        {v1.z - v2.z,v1.z - v3.z,d.z}
    };

    float beta_ = det3x3(beta_top) / det3x3(A);
    float gamma = det3x3(gamma_top) / det3x3(A);
    float t = det3x3(t_top) / det3x3(A);


    if(beta_ + gamma <= 1 && beta_ >= 0 && gamma >= 0)
        return t;

   return -1;


}

Vec3 Triangle::getSurfaceNormal(Ray r){
    


    Vec3 v1 = this->tm->transform(this->v1);
    Vec3 v2 = this->tm->transform(this->v2);
    Vec3 v3 = this->tm->transform(this->v3);

    Vec3 scaled_n = (this->v3 - this->v1).cross((this->v2-this->v1));

    return (v3 - v1).cross(v2 - v1).normalize();

}

Triangle::Triangle(){

}
Vec3 Triangle::getMotionBlur(){
    return this->motionBlur;
}

Triangle::Triangle(Material* material, ObjectType objectType, Vec3 v1, Vec3 v2 , Vec3 v3, TransformationMatrix* tm, Mesh* mesh, int num_tex_maps, TextureMap* texture_maps, std::vector<std::pair<float, float> > uv_coords_triangle)
{
    this->material = material;
    this->objectType = objectType;
    this->mesh = mesh;
    this->tex_flags = texture_flags();
    this->num_tex_maps = num_tex_maps;
    this->uv_coords_triangle = uv_coords_triangle;
    for (size_t i = 0; i < num_tex_maps; i++)
    {
        if (texture_maps[i].decal_mode == replace_kd) {
            this->tex_flags.replace_kd = true;
            this->tex_flags.replace_kd_texture = &texture_maps[i];
        } else if (texture_maps[i].decal_mode == blend_kd) {
            this->tex_flags.blend_kd = true;
            this->tex_flags.blend_kd_texture = &texture_maps[i];
        } else if (texture_maps[i].decal_mode == replace_ks) {
            this->tex_flags.replace_ks = true;
            this->tex_flags.replace_ks_texture = &texture_maps[i];
        } else if (texture_maps[i].decal_mode == bump_normal) {
            this->tex_flags.bump_normal = true;
            this->tex_flags.bump_normal_texture = &texture_maps[i];
        } else if (texture_maps[i].decal_mode == replace_all) {
            this->tex_flags.replace_all = true;
            this->tex_flags.replace_all_texture = &texture_maps[i];
        } else if (texture_maps[i].decal_mode == replace_normal) {
            this->tex_flags.replace_normal = true;
            this->tex_flags.replace_normal_texture = texture_maps + i;
        }else {
            // Handle any unrecognized decal modes if necessary.
            std::cerr << "Unknown decal mode encountered: " << texture_maps[i].decal_mode << std::endl;
        }
    }

    this->tex_flags.any = this->tex_flags.replace_kd && this->tex_flags.blend_kd && this->tex_flags.replace_ks && this->tex_flags.bump_normal && this->tex_flags.replace_all && this->tex_flags.replace_normal;
    this->tm =  tm;

    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;

};

Vec3 Triangle::getBoundingBox(bool isMax){

    Vec3 v1 = this->tm->transform(this->v1);
    Vec3 v2 = this->tm->transform(this->v2);
    Vec3 v3 = this->tm->transform(this->v3);
    
    Vec3 minBound;
    Vec3 maxBound;
    minBound.x = std::min({v1.x, v2.x, v3.x});
    minBound.y = std::min({v1.y, v2.y, v3.y});
    minBound.z = std::min({v1.z, v2.z, v3.z});

    // Calculate maximum bounds for x, y, z
    maxBound.x = std::max({v1.x, v2.x, v3.x});
    maxBound.y = std::max({v1.y, v2.y, v3.y});
    maxBound.z = std::max({v1.z, v2.z, v3.z});

    Vec3 min = minBound;
    Vec3 max = maxBound;

    if(isMax)
        return max;
    return min;
}

Vec3  Triangle::getCenter(){
    return this->tm->transform((this->v1 + this-> v2 + this->v3)/ 3.0f) ;
}



Material* Triangle::getMaterial(){
    return this->material;
}
ObjectType Triangle::getObject(){
    return this->objectType;
}



int Triangle::get_num_tex_maps(){
    return this->num_tex_maps;
}
TextureMap* Triangle::get_texture_maps() {
    return this->texture_maps;
}
texture_flags Triangle::get_texture_flags(){
    return this->tex_flags;
}