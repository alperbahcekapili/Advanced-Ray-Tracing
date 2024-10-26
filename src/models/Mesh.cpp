

#include "Mesh.h"
#include "../util/util.h"

Mesh::Mesh(Material* material, ObjectType objectType, Vec3* faces, int numfaces):
material(material), objectType(objectType){
    this->faces = new Triangle[numfaces];
    for (int i = 0; i < numfaces; i++)
    {
        this->faces[i] = Triangle(
            material, ObjectType::TriangleType, 
            faces[i*3], faces[i*3+1], faces[i*3+2]
        );
    }
    this->num_faces = numfaces;
    this->objectType = objectType;
}

Mesh::~Mesh(){
    delete[] faces;
}



float Mesh::Intersects(Ray ray){
// if a face of mesh intersects with the  ray return that
float mint = 999999;
int intersecting_face_index = -1;
for (int i = 0; i < num_faces; i++)
{  
   float intersecting_t = this->faces[i].Intersects(ray);
   if (intersecting_t < mint && intersecting_t > 0){
    mint = intersecting_t;
    intersecting_face_index = i;
   }

}
if (intersecting_face_index == -1)
    return -1.0;
    // std::cout << "Mint for mesh: " << mint << ", Intersecting face index: " << intersecting_face_index <<"\n";
return mint;
}


Vec3 Mesh::getSurfaceNormal(Vec3 location) {
    // get closest triangle and calculate normal of that triangle
    int mindist = 999999;
    int intersecting_face_index=-1;
    for (int i = 0; i < num_faces; i++)
    {  
        Vec3 center = (this->faces[i].v1 + this->faces[i].v2 + this->faces[i].v3) / 3.0 ;
        float dist = (center - location).magnitude();
        if(dist < mindist){
            mindist = dist;
            intersecting_face_index = i;
        }


    }
    return this->faces[intersecting_face_index].getSurfaceNormal(location);
}


Material* Mesh::getMaterial(){
    return this->material;
}
ObjectType Mesh::getObject(){
    return this->objectType;
}



