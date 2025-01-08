#include <chrono>

#include "Mesh.h"
#include "../util/util.h"
#include "../acceleration/BVH.h"



Mesh::Mesh(Material* material, ObjectType objectType, Vec3* faces, int numfaces, TransformationMatrix* tm, int num_tex_maps, TextureMap* texture_maps, std::vector<std::pair<float, float> > uv_coords_mesh):
 objectType(objectType){
    this->material = material;
    this->num_tex_maps = num_tex_maps;
    // construct tmap_flags objects as well
    this->tex_flags = texture_flags();
    this->texture_maps = texture_maps;
    this->uv_coords_mesh = uv_coords_mesh;
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
    // std::cout << "Material:"<< material->materialType << std::endl;
    this->tm = new TransformationMatrix();
    *(this->tm) = *(tm);
    this->last_intersected_obj = nullptr;
    Object** triangles = new Object*[numfaces];
    this->faces = new Object*[numfaces];
    float centerx = 0;
    float centery = 0;
    float centerz = 0;
    for (size_t i = 0; i < (numfaces * 3); i++)
    {
        centerz += faces[i].z / float(numfaces*3);
        centery += faces[i].y / float(numfaces*3);
        centerx += faces[i].x / float(numfaces*3);
    }
    
    this->center = Vec3(centerx, centery, centerz);
    // std::cout << "Mesh Center Before Transform: " << this->center.x << ", " << this->center.y << ", " << this->center.z << "\n";
    
    TransformationMatrix* resulting_tm = tm;
    
    for (int i = 0; i < numfaces; i++)
    {
        
        std::vector<std::pair<float, float> > uv_coords_triangle;
        if(num_tex_maps>0){
        uv_coords_triangle.push_back(uv_coords_mesh.at(i*3));
        uv_coords_triangle.push_back(uv_coords_mesh.at(i*3+1));
        uv_coords_triangle.push_back(uv_coords_mesh.at(i*3+2));}
        // Initialize triangles with transformation relative to mesh center
        Triangle* tmp_triangle = new Triangle(
            material, ObjectType::TriangleType, 
            faces[i*3], faces[i*3+1], faces[i*3+2], 
            resulting_tm, this, num_tex_maps, texture_maps, uv_coords_triangle
        );
        tmp_triangle->mesh = this;
        triangles[i] = tmp_triangle;
        this->faces[i] = tmp_triangle;
    }
    this->num_faces = numfaces;
    this->bvh_faces = new BVH(triangles, num_faces, 0);
    
    this->objectType = objectType;
    // this->center = resulting_tm->transform(this->center);
    // std::cout << "Mesh Center After Transform: " << this->center.x << ", " << this->center.y << ", " << this->center.z << "\n";
}

Mesh::~Mesh(){
    delete[] faces;
}


float Mesh::Intersects(Ray ray){
// if a face of mesh intersects with the  ray return that
float mint, maxt;
Object* tofill;


auto start = std::chrono::high_resolution_clock::now();
bool intersects = this->bvh_faces->intersectObject(ray, tofill, mint, maxt);
auto end = std::chrono::high_resolution_clock::now();

// Calculate duration
std::chrono::duration<double> duration = end - start;

if(intersects){
    this->last_intersected_obj = tofill;
    return mint;
};
return -1.0;
}

TransformationMatrix* Mesh::gettm() {
    return this->tm;
}

Vec3 Mesh::getMotionBlur(){
    return this->motionBlur;
}

Vec3 Mesh::getSurfaceNormal(Ray r) {
    // get closest triangle and calculate normal of that triangle

    float mint = this->Intersects(r);
    if(mint == -1)
        return Vec3(-1,-1,-1);

    return this->last_intersected_obj->getSurfaceNormal(r);
}


Material* Mesh::getMaterial(){
    return this->material;
}
ObjectType Mesh::getObject(){
    return this->objectType;
}




Vec3 Mesh::getBoundingBox(bool isMax){
    if(isMax)
        return this->bvh_faces->max;
    return this->bvh_faces->min;

}
Vec3  Mesh::getCenter(){
    return this->center;
}


int Mesh::get_num_tex_maps(){
    return this->num_tex_maps;
}
TextureMap* Mesh::get_texture_maps() {
    return this->texture_maps;
}
texture_flags Mesh::get_texture_flags(){
    return this->tex_flags;
}