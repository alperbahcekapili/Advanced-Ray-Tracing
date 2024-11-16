

#include "Mesh.h"
#include "../util/util.h"
#include "../acceleration/BVH.h"



Mesh::Mesh(Material* material, ObjectType objectType, Vec3* faces, int numfaces, TransformationMatrix* tm):
 objectType(objectType){
    this->material = material;
    
    // std::cout << "Material:"<< material->materialType << std::endl;
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
    // std::cout << "Mesh Center Before Transform: " << this->center.x << ", " << this->center.y << ", " << this->center.z << "\n";
    
    center = center * -1;
    TransformationMatrix* to_center = new TransformationMatrix(center, 't');
    center = center * -1;
    TransformationMatrix* from_center = new TransformationMatrix(center, 't');
    TransformationMatrix* resulting_tm = new TransformationMatrix();
    *resulting_tm = (*from_center)  * (*tm) ;
    *resulting_tm = (*resulting_tm) * (*to_center);
    //  for (int i = 0; i < 4; i++) {
    //     for (int j = 0; j < 4; j++) {
    //         // std::cout << resulting_tm->matrix[i][j] << "\t"; // Tab for spacing
    //     }
    //     // std::cout << std::endl; // Newline after each row
    // }
    
    for (int i = 0; i < numfaces; i++)
    {
        // Initialize triangles with transformation relative to mesh center
        Triangle* tmp_triangle = new Triangle(
            material, ObjectType::TriangleType, 
            faces[i*3], faces[i*3+1], faces[i*3+2], 
            resulting_tm
        );
        tmp_triangle->mesh = this;
        triangles[i] = tmp_triangle;
    }
    this->num_faces = numfaces;
    this->bvh_faces = new BVH(triangles, num_faces, 0);
    
    this->objectType = objectType;
    this->center = resulting_tm->transform(this->center);
    // std::cout << "Mesh Center After Transform: " << this->center.x << ", " << this->center.y << ", " << this->center.z << "\n";
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

