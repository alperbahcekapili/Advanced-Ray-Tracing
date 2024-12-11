
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
    /*
    
    */ 


    float gamma_top[3][3] = {
        {this->v1.x - this->v2.x, this->v1.x - o.x, d.x},
        {this->v1.y - this->v2.y, this->v1.y - o.y, d.y},
        {this->v1.z - this->v2.z, this->v1.z - o.z, d.z}
    };

    float beta_top[3][3] = {
        {this->v1.x - o.x, this->v1.x - this->v3.x, d.x},
        {this->v1.y - o.y, this->v1.y - this->v3.y, d.y},
        {this->v1.z - o.z, this->v1.z - this->v3.z, d.z}
    };

    float t_top[3][3] = {
        {this->v1.x - this->v2.x, this->v1.x - this->v3.x, this->v1.x - o.x},
        {this->v1.y - this->v2.y, this->v1.y - this->v3.y, this->v1.y - o.y},
        {this->v1.z - this->v2.z, this->v1.z - this->v3.z, this->v1.z - o.z}
    };

    float A[3][3] = {
        {this->v1.x - this->v2.x,this->v1.x - this->v3.x,d.x},
        {this->v1.y - this->v2.y,this->v1.y - this->v3.y,d.y},
        {this->v1.z - this->v2.z,this->v1.z - this->v3.z,d.z}
    };

    float beta_ = det3x3(beta_top) / det3x3(A);
    float gamma = det3x3(gamma_top) / det3x3(A);
    float t = det3x3(t_top) / det3x3(A);


    if(beta_ + gamma <= 1 && beta_ >= 0 && gamma >= 0)
        return t;

   return -1;


}

Vec3 Triangle::getSurfaceNormal(Ray r){
    if(!this->tex_flags.replace_normal)
    return this->normal;


    if(!this->tbset)
    {
        Vec3 E1 = this->v2 - this->v1;
        Vec3 E2 = this->v3 - this->v1;

        // UV edge vectors
        float  deltaUV1x = this->uv_coords_triangle.at(1).first - this->uv_coords_triangle.at(0).first;
        float  deltaUV1y = this->uv_coords_triangle.at(1).second - this->uv_coords_triangle.at(0).second;
        float  deltaUV2x = this->uv_coords_triangle.at(2).first - this->uv_coords_triangle.at(0).first;
        float  deltaUV2y = this->uv_coords_triangle.at(2).second - this->uv_coords_triangle.at(0).second;
        

        // Compute the determinant
        float determinant = deltaUV1x * deltaUV2y - deltaUV1y * deltaUV2x;

        float invDet = 1.0f / determinant;

        // Compute T and B
        Vec3 T = invDet * (deltaUV2y * E1 - deltaUV1y * E2);
        Vec3 B = invDet * (-deltaUV2x * E1 + deltaUV1x * E2);

        // Normalize T and B
        this->T = T.normalize();
        this->B = B.normalize();
        this->tbset = true;
    }
    
    // transform normall according to texture value
    float tval = this->Intersects(r);
    Vec3 inters_loc = r.locationAtT(tval);
    uv uv_loc = uv::calculateUVTriangle(inters_loc, this->v1, this->v2, this->v3, this->uv_coords_triangle.at(0), this->uv_coords_triangle.at(1), this->uv_coords_triangle.at(2));
    Vec3 tex_value = this->tex_flags.replace_normal_texture->interpolateAt(uv_loc, tex_flags.replace_normal_texture->interpolation_type);
    tex_value = (tex_value * 2) - Vec3(1,1,1);
    tex_value = tex_value.normalize(); 

    Vec3 nnew = T*tex_value + B*tex_value + this->normal*tex_value;
    
    return nnew.normalize();





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
    

    // before setting tm we need to move the center to origin then move back
    this->tm = new TransformationMatrix();
    Vec3 center = (v1 +  v2 + v3 ) / 3.0f;
    // std::cout << "Triangle before transform center: " << center.x << ", " << center.y << ", " << center.z << "\n";

    // if this is not a face then do op. relative to center
    if(this->mesh == nullptr){
        Vec3 center = (v1 +  v2 + v3 ) / 3.0f;
        TransformationMatrix* to_center = new TransformationMatrix(-1*center, 't');
        TransformationMatrix* from_center = new TransformationMatrix(center, 't');
        *(this->tm) = (*from_center) * (*tm) * (*to_center);}
    else
    *(this->tm) =  (*tm);
    

    this->v1 = this->tm->transform(v1);
    this->v2 = this->tm->transform(v2);
    this->v3 = this->tm->transform(v3);
    
    Vec3 new_center = (this->v1 +  this->v2 + this->v3 ) / 3.0f;
    
    // std::cout << "Center: " << new_center.x << ", " << new_center.y << ", " << new_center.z << "\n";


    Vec3 scaled_n = (this->v2-this->v1).cross(this->v3 - this->v1);
    bool det_negative = this->tm->determinant() < 0;
    if(det_negative)
        scaled_n = (this->v3 - this->v1).cross(this->v2-this->v1);

    this->normal = scaled_n.normalize();

    Vec3 minBound;
    Vec3 maxBound;
    minBound.x = std::min({this->v1.x, this->v2.x, this->v3.x});
    minBound.y = std::min({this->v1.y, this->v2.y, this->v3.y});
    minBound.z = std::min({this->v1.z, this->v2.z, this->v3.z});

    // Calculate maximum bounds for x, y, z
    maxBound.x = std::max({this->v1.x, this->v2.x, this->v3.x});
    maxBound.y = std::max({this->v1.y, this->v2.y, this->v3.y});
    maxBound.z = std::max({this->v1.z, this->v2.z, this->v3.z});


    this->min = minBound;
    this->max = maxBound;
    

};

Vec3 Triangle::getBoundingBox(bool isMax){
    if(isMax)
        return this->max;
    return this->min;
}

Vec3  Triangle::getCenter(){
    return (this->v1 + this-> v2 + this->v3 ) / 3.0f;
}

// Function to calculate the area of this->v1 triangle given its vertices
float Triangle::getArea(void) {
    // Vectors AB and AC
    
    Vec3 AB(this->v2.x - this->v1.x, this->v2.y - this->v1.y, this->v2.z - this->v1.z);
    Vec3 AC(this->v3.x - this->v1.x, this->v3.y - this->v1.y, this->v3.z - this->v1.z);

    // Cross product of AB and AC
    Vec3 cross = AB.cross(AC);

    // Area is half the magnitude of the cross product
    return 0.5 * cross.magnitude();
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