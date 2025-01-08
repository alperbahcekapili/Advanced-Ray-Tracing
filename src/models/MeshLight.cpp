

#include "Object.h"
#include "Triangle.h"
#include <array>
#include "../acceleration/BVH.h"
#include "MeshLight.h"
#include "../util/util.h"

MeshLight::MeshLight(Material* material, ObjectType objectType, Vec3* faces, int numfaces, TransformationMatrix* tm, int num_tex_maps, TextureMap* texture_maps, std::vector<std::pair<float, float> > uv_coords_mesh, Vec3 radiance)
: Mesh( material, objectType,  faces, numfaces, tm, num_tex_maps, texture_maps, uv_coords_mesh){
    
    this->objectType = MeshLightType;
    this->material = material;
    this->tm = tm;
    this->tex_flags.any = false;

    this->area = 0;
    this->tm = new TransformationMatrix();
    *(this->tm) = *(tm);
    this->last_intersected_obj = nullptr;
    Object** triangles = new Object*[numfaces];

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
    TransformationMatrix* resulting_tm = tm;
    for (int i = 0; i < numfaces; i++)
    {   
        std::vector<std::pair<float, float> > uv_coords_triangle;
        Triangle* tmp_triangle = new Triangle(
            material, ObjectType::TriangleLightType, 
            faces[i*3], faces[i*3+1], faces[i*3+2], 
            resulting_tm, this, num_tex_maps, texture_maps, uv_coords_triangle
        );
        tmp_triangle->radiance = radiance;
        this->area += tmp_triangle->getArea();
        tmp_triangle->mesh = this;
        triangles[i] = tmp_triangle;
    }

    this->num_faces = numfaces;
    this->bvh_faces = new BVH(triangles, num_faces, 0);
    this->radiance = radiance;
}





// TODO Update below functions
Vec3  MeshLight::irradianceAt(Ray light_ray, Vec3  location)  {
    Vec3 sample_loc = light_ray.o;
    Vec3 light_direction = light_ray.d;

    float distance = (location - sample_loc).magnitude();
    Triangle* selected_tri = dynamic_cast<Triangle*>(this->last_intersected_obj);
    float eq_top = selected_tri->normal.dot(light_direction);
    float dwi = this->area * eq_top / (distance*distance);

    return this->radiance * dwi;


}

Vec3 MeshLight::getPointOn(){
    // sample random location 
    int random_tri_index = int(generate_random_01() * this->num_faces);
    Triangle* selected_tri = dynamic_cast<Triangle*>(this->faces[random_tri_index]);
    this->last_intersected_obj = selected_tri;
    // sample 3 random numbers to get a point on the triangle
    float r1 = generate_random_01();
    float r2 = generate_random_01();
    Vec3 point_on_tri = selected_tri->v1 + r1 * (selected_tri->v2 - selected_tri->v1) + r2 * (selected_tri->v3 - selected_tri->v1);
    return point_on_tri;
}




MeshLight::~MeshLight(){}