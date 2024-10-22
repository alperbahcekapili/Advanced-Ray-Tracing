#include "Mesh.h"
#include "../util/util.h"

Mesh::Mesh(Material* material, ObjectType objectType, std::vector<std::array<std::array<float, 3>, 3> > faces, int numfaces):
material(material), objectType(objectType){
    for (int i = 0; i < numfaces; i++)
    {
        this->faces.push_back(Triangle(
            material, ObjectType::TriangleType, 
            
            
            faces.at(i)[0].data(), faces.at(i)[1].data(), faces.at(i)[2].data()
        ));
    }
    this->num_faces = numfaces;
    this->objectType = objectType;
}



float Mesh::Intersects(Ray ray){
// if a face of mesh intersects with the  ray return that
float mint = 999999;
int intersecting_face_index = -1;
for (int i = 0; i < num_faces; i++)
{  
   float intersecting_t = this->faces.at(i).Intersects(ray);
   if (intersecting_t < mint && intersecting_t > 0){
    mint = intersecting_t;
    intersecting_face_index = i;
   }

}
if (intersecting_face_index == -1)
    return -1.0;
    std::cout << "Mint for mesh: " << mint << ", Intersecting face index: " << intersecting_face_index <<"\n";
return mint;
}


vector<float> Mesh::getSurfaceNormal(vector<float> location) {
    // get closest triangle and calculate normal of that triangle
    int mindist = 999999;
    int intersecting_face_index;
    for (int i = 0; i < num_faces; i++)
    {  
        std::vector<float> center = {
            (this->faces.at(i).v1[0] + this->faces.at(i).v2[0] + this->faces.at(i).v3[0])/3,
            (this->faces.at(i).v1[1] + this->faces.at(i).v2[1] + this->faces.at(i).v3[1])/3,
            (this->faces.at(i).v1[2] + this->faces.at(i).v2[2] + this->faces.at(i).v3[2])/3};
        float dist = getMagnitude(vectorSubstract(center, location));
        if(dist < mindist){
            mindist = dist;
            intersecting_face_index = i;
        }


    }
    return this->faces.at(0).getSurfaceNormal(location);

}


Material* Mesh::getMaterial(){
    return this->material;
}
ObjectType Mesh::getObject(){
    return this->objectType;
}



