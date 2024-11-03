

#include "Mesh.h"
#include "../util/util.h"
#include "../acceleration/BVH.h"



Mesh::Mesh(Material* material, ObjectType objectType, Vec3* faces, int numfaces, TransformationMatrix* tm):
material(material), objectType(objectType){
    this->tm = new TransformationMatrix();
    *(this->tm) = *(tm);
    this->last_intersected_obj = nullptr;
    Object** triangles = new Object*[numfaces];

    float centerx, centery, centerz;
    for (size_t i = 0; i < numfaces; i++)
    {
        centerz += faces[i*3+2].x / numfaces;
        centery += faces[i*3+1].x / numfaces;
        centerx += faces[i*3].x / numfaces;
    }
    
    this->center = Vec3(centerx, centery, centerz);

    TransformationMatrix* to_center = new TransformationMatrix(-1*center, 't');
    TransformationMatrix* from_center = new TransformationMatrix(center, 't');

    


    for (int i = 0; i < numfaces; i++)
    {
        // Initialize triangles with transformation relative to mesh center
        TransformationMatrix resulting_tm = ((*from_center)  * (*tm) * (*to_center));
        Triangle* tmp_triangle = new Triangle(
            material, ObjectType::TriangleType, 
            faces[i*3], faces[i*3+1], faces[i*3+2], 
            &resulting_tm
        );
        tmp_triangle->mesh = this;
        triangles[i] = tmp_triangle;
    }
    this->num_faces = numfaces;
    this->bvh_faces = new BVH(triangles, num_faces, 0);
    
    this->objectType = objectType;
}

Mesh::~Mesh(){
    delete[] faces;
}


float Mesh::Intersects(Ray ray){
// if a face of mesh intersects with the  ray return that
float mint, maxt;
Object* tofill;
bool intersects = this->bvh_faces->intersectObject(ray, tofill, mint, maxt);
    // std::cout << "Mint for mesh: " << mint << ", Intersecting face index: " << intersecting_face_index <<"\n";
if(intersects){
    this->last_intersected_obj = tofill;
    return mint;
};
return -1.0;
}


Vec3 Mesh::getSurfaceNormal(Ray r) {
    // get closest triangle and calculate normal of that triangle

    float mint = this->Intersects(r);
    if(mint == -1)
        return Vec3(-1,-1,-1);

    Vec3 location = r.locationAtT(mint);
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

