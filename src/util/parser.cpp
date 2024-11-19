#include "parser.h"
#include "tinyxml2.h"
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <iostream>

#include "../models/Camera.h"
#include "../lights/Light.h"
#include "../lights/PointLight.h"
#include "../models/Material.h"
#include "../models/Mesh.h"
#include "../models/Sphere.h"
#include "../models/ImagePane.h"
#include "../models/ObjectInstance.h"
#include "../scene/Scene.h"
#include "data_structures.h"
#include "ply.h"
#include "read_ply.h"



std::vector<Scene*> loadFromXml(const std::string &filepath)
{
    tinyxml2::XMLDocument file;
    std::stringstream stream;
    
    auto res = file.LoadFile(filepath.c_str());
    float shadow_ray_eps = 0;

    // Main variables with Vec3 where appropriate
    Vec3 background_color(0, 0, 0);
    int max_recursion_depth = 0;

    Vec3 cam_position(0, 0, 0);
    Vec3 cam_gaze(0, 0, 0);
    Vec3 cam_up(0, 0, 0);
    Vec4 cam_near_plane(0, 0, 0, 0);
    float cam_near_distance = 0;
    int img_width;
    int img_height;
    std::string camera_name;
    std::vector<Camera*> cameras;

    Vec3 ambient_light(0, 0, 0);
    std::vector<PointLight*> lights;
    Vec3 light_position(0, 0, 0);
    Vec3 light_intensity(0, 0, 0);

    std::vector<Material*> materials;
    MaterialType material_type;
    Vec3 ambient_reflectance(0, 0, 0);
    Vec3 diffuse_reflectance(0, 0, 0);
    Vec3 specular_reflectance(0, 0, 0);
    Vec3 mirror_reflectance(0, 0, 0);
    Vec3 absorption_coefficient(0, 0, 0);
    float absorption_index;
    bool is_mirror;
    float phong_exponent;
    float refraction_index;

    std::vector<Vec3> vertices;      // Each vertex can be a Vec3
    Vec3 cur_vertex(1, 1, 1);

    std::vector<int> mesh_vertex_ids = {0, 0, 0};

    Material* mesh_material;
    int mesh_material_id;
    int facevid1, facevid2, facevid3;
    std::vector<Vec3> mesh_faces;
    int mesh_numfaces = 0;
    std::vector<Object*> meshes;


    int triangle_material_id;
    int trianglev1, trianglev2, trianglev3;
    std::vector<Triangle*> triangles;


    int sphere_material_id;
    float shpere_radius;
    int sphere_center_id;
    std::vector<Sphere*> spheres;




    std::vector<Vec3> scale;
    std::vector<Vec3> translate;
    std::vector<Vec3> rotate;

    
    


    if (res)
    {
        throw std::runtime_error("Error: The xml file cannot be loaded.");
    }

    auto root = file.FirstChild();
    if (!root)
    {
        throw std::runtime_error("Error: Root is not found.");
    }

    //Get BackgroundColor
    auto element = root->FirstChildElement("BackgroundColor");
    if (element)
        stream << element->GetText() << std::endl;
    else
        stream << "0 0 0" << std::endl;
    
    stream >> background_color.x >> background_color.y >> background_color.z;
    
    stream.clear();
    stream.str("");

    element = root->FirstChildElement("ShadowRayEpsilon");
    if (element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0.001" << std::endl;
    }
    
    stream >> shadow_ray_eps;


    element = root->FirstChildElement("MaxRecursionDepth");
    if (element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "1" << std::endl;
    }
    stream >> max_recursion_depth;

    element = root->FirstChildElement("Cameras");
    element = element->FirstChildElement("Camera");
    


    while (element)
    {
        tinyxml2::XMLElement* child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Gaze");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Up");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearPlane");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearDistance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageResolution");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageName");
        stream << child->GetText() << std::endl;

        stream >>  cam_position.x >> cam_position.y >> cam_position.z;
        stream >> cam_gaze.x >> cam_gaze.y >> cam_gaze.z;
        stream >> cam_up.x >> cam_up.y >> cam_up.z;
        stream >> cam_near_plane.x >> cam_near_plane.y >> cam_near_plane.z >> cam_near_plane.h;
        stream >> cam_near_distance;
        stream >> img_width >> img_height;
        stream >> camera_name;
        // max t hardcoded
        Camera* camera = new Camera(
            cam_up, cam_gaze , cam_position, cam_near_distance, 1000, camera_name
        );
        cameras.push_back(camera);
        element = element->NextSiblingElement("Camera");
    }

    
    element = root->FirstChildElement("Lights");
    tinyxml2::XMLElement* child = element->FirstChildElement("AmbientLight");
    stream << child->GetText() << std::endl;
    stream >> ambient_light.x >> ambient_light.y >> ambient_light.z;
    element = element->FirstChildElement("PointLight");


    while (element)
    {
        child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Intensity");
        stream << child->GetText() << std::endl;

        stream >> light_position.x >> light_position.y >> light_position.z;
        stream >> light_intensity.x >> light_intensity.y >> light_intensity.z;
        PointLight* l = new PointLight(light_intensity, light_position);
        lights.push_back(l);
        element = element->NextSiblingElement("PointLight");
    }


    element = root->FirstChildElement("Materials");
    element = element->FirstChildElement("Material");
    
    while (element)
    {
        is_mirror = (element->Attribute("type", "mirror") != NULL);
        bool is_conductor = (element->Attribute("type", "conductor") != NULL);
        bool is_dielectric = (element->Attribute("type", "dielectric") != NULL);
        if (is_mirror)
            material_type = MaterialType::Mirror;
        else if (is_conductor)
            material_type = MaterialType::Conductor;
        else if (is_dielectric)
            material_type = MaterialType::Dielectric;

        if (!is_mirror && !is_dielectric && !is_conductor)
        material_type = MaterialType::Other;


        child = element->FirstChildElement("AmbientReflectance");
        stream << child->GetText() << std::endl;

        child = element->FirstChildElement("DiffuseReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("SpecularReflectance");
        stream << child->GetText() << std::endl;


        //
        child = element->FirstChildElement("RefractionIndex");
        if(child)
            stream << child->GetText() << std::endl;
        else
            stream << 0 << std::endl;


        child = element->FirstChildElement("AbsorptionCoefficient");
        if (child)
        stream << child->GetText() << std::endl;
        else
        stream << "0 0 0 \n" << std::endl;

        child = element->FirstChildElement("AbsorptionIndex");
        if (child)
        stream << child->GetText() << std::endl;
        else
        stream << 0 << std::endl;

        child = element->FirstChildElement("MirrorReflectance");
        if (child)
        {
            stream << child->GetText() << std::endl;
        }else{
            stream << "0 0 0 \n" << std::endl;
        }
        child = element->FirstChildElement("PhongExponent");
        if(child)
        stream << child->GetText() << std::endl;
        else
        stream << 1 << std::endl;


        stream >> ambient_reflectance.x >> ambient_reflectance.y >> ambient_reflectance.z;
        stream >> diffuse_reflectance.x >> diffuse_reflectance.y >> diffuse_reflectance.z;
        stream >> specular_reflectance.x >>  specular_reflectance.y >>  specular_reflectance.z;
        stream >> refraction_index;
        stream >> absorption_coefficient.x >>  absorption_coefficient.y >>  absorption_coefficient.z;
        stream >> absorption_index;
        stream >> mirror_reflectance.x >> mirror_reflectance.y>> mirror_reflectance.z;
        
        stream >> phong_exponent;
        Material* material = new Material(material_type, ambient_reflectance, diffuse_reflectance, specular_reflectance, phong_exponent, mirror_reflectance, refraction_index, absorption_index, absorption_coefficient);
        materials.push_back(material);
        element = element->NextSiblingElement("Material");
    }
    stream.clear();  // Clear any potential error state flags
    stream.str(""); // Clear the stream content
    
    element = root->FirstChildElement("Transformations");
    if(element){
        
    stream.clear();  // Clear any potential error state flags
    stream.str(""); // Clear the stream content
    float transformation_buf1, transformation_buf2, transformation_buf3, transformation_buf4;
    auto scale_elem = element->FirstChildElement("Scaling");
    while(scale_elem){
        string tmp = scale_elem->GetText();
        stream << tmp << std::endl;
        stream >> transformation_buf1 >> transformation_buf2 >> transformation_buf3;
        scale.push_back(Vec3(transformation_buf1, transformation_buf2, transformation_buf3));
        scale_elem = scale_elem->NextSiblingElement("Scaling");
    }
    auto translate_elem = element->FirstChildElement("Translation");
    while(translate_elem){
        string tmp = translate_elem->GetText();
        stream << tmp << std::endl;
        stream >> transformation_buf1 >> transformation_buf2 >> transformation_buf3;
        translate.push_back(Vec3(transformation_buf1, transformation_buf2, transformation_buf3));
        translate_elem = translate_elem->NextSiblingElement("Translation");
    }
    auto rotate_elem = element->FirstChildElement("Rotation");
    while(rotate_elem){
        string tmp = rotate_elem->GetText();
        stream << tmp << std::endl;
        stream >> transformation_buf1 >> transformation_buf2 >> transformation_buf3 >>  transformation_buf4;
        rotate.push_back(Vec3(transformation_buf2*transformation_buf1, transformation_buf3*transformation_buf1, transformation_buf4*transformation_buf1));
        rotate_elem = rotate_elem->NextSiblingElement("Rotation");
    }}
    
    

    element = root->FirstChildElement("VertexData");
    stream << element->GetText() << std::endl;
    
    while (stream >> cur_vertex.x >> cur_vertex.y >> cur_vertex.z)
    {
        vertices.push_back(cur_vertex);
    }
    stream.clear();
    stream.str("");

    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Mesh");
    while (element)
    {
        child = element->FirstChildElement("Material");
        stream.clear();
        string tmp = child->GetText();
        stream << tmp << std::endl;
        stream >> mesh_material_id;
        mesh_material = materials.at(mesh_material_id-1);

        std::vector<TransformationMatrix*> tms;
        child = element->FirstChildElement("Transformations");
        if(child){
            stream << child->GetText() << std::endl;
            string transformation_buffer;
            while(stream >> transformation_buffer){
                char op = transformation_buffer.at(0);
                int id = std::stoi(transformation_buffer.substr(1));
                if(op=='r'){
                    tms.push_back(new TransformationMatrix(rotate.at(id-1), op));
                }else if (op=='t')
                {
                    tms.push_back(new TransformationMatrix(translate.at(id-1), op));
                }else if (op=='s')
                {
                    tms.push_back(new TransformationMatrix(scale.at(id-1), op));
                }
            }
        }
        // Initialize resulting tm as identity matrix so that it means no transformation
        
        TransformationMatrix* resulting_tm = new TransformationMatrix();
        resulting_tm->matrix[0][0] = 1;
        resulting_tm->matrix[1][1] = 1;
        resulting_tm->matrix[2][2] = 1;
        resulting_tm->matrix[3][3] = 1;
        for (int i = tms.size()-1; i >= 0; i--)
        {
            *resulting_tm =   (*resulting_tm) * (*tms.at(i));
        }
            
        
        TransformationMatrix* resulting_tm_copy;
        child = element->FirstChildElement("Faces");
        stream.clear();
        int numfaces;
        char *** elem_names;
        int ftype;
        float version;
        
        char* plyattr = "plyFile";
		if (child->Attribute(plyattr))
		{
			std::string plyFilePath = child->Attribute("plyFile");
            std::size_t firstSlashPos = filepath.rfind('/');
                if (firstSlashPos == std::string::npos) {
            firstSlashPos = -1;
            }
            // Construct the new path: part before the first '/' + '/' + new filename
            plyFilePath = filepath.substr(0, firstSlashPos + 1) + plyFilePath;
            std::vector<Vec3> tmp_l = read_binary_ply(plyFilePath);
            mesh_faces.insert(mesh_faces.end(), tmp_l.begin(), tmp_l.end());
		}
		else
		{
			stream << child->GetText() << std::endl;
            while (!(stream >> facevid1).eof())
            {   
                stream >> facevid2 >> facevid3;

                // here create mesh
                mesh_faces.push_back(vertices.at(facevid1-1));
                mesh_faces.push_back(vertices.at(facevid2-1));
                mesh_faces.push_back(vertices.at(facevid3-1));
                    
            }

		}
       
      stream.clear();
        // convert vector to list
        Vec3* mesh_faces_ar = new Vec3[mesh_faces.size()];
        for (int i = 0; i < mesh_faces.size(); i++)
        {
            mesh_faces_ar[i] = mesh_faces.at(i);
            // std::cout << i << "\n" ;
        }
        mesh_numfaces = int(mesh_faces.size()/3);
        Mesh* m = new Mesh(materials.at(mesh_material_id-1), ObjectType::MeshType, mesh_faces_ar, mesh_numfaces, resulting_tm);
        meshes.push_back(m);

        mesh_faces.clear();
        element = element->NextSiblingElement("Mesh");
    }
    




    // //Get Triangles
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Triangle");

    while (element)
    {
        child = element->FirstChildElement("Material");
        stream <<  child->GetText() << std::endl;
        stream >> triangle_material_id;
    
        
        // create 4x4 trasformation matrix for each of the transformation
        std::vector<TransformationMatrix*> tms;
        child = element->FirstChildElement("Transformations");
        if(child){
            stream << child->GetText() << std::endl;
            string transformation_buffer;
            while(stream >> transformation_buffer){
                char op = transformation_buffer.at(0);
                int id = std::stoi(transformation_buffer.substr(1));
                if(op=='r'){
                    tms.push_back(new TransformationMatrix(rotate.at(id-1), op));
                }else if (op=='t')
                {
                    tms.push_back(new TransformationMatrix(translate.at(id-1), op));
                }else if (op=='s')
                {
                    tms.push_back(new TransformationMatrix(scale.at(id-1), op));
                }
            }
        }
        // Initialize resulting tm as identity matrix so that it means no transformation
        TransformationMatrix* resulting_tm = new TransformationMatrix();
        resulting_tm->matrix[0][0] = 1;
        resulting_tm->matrix[1][1] = 1;
        resulting_tm->matrix[2][2] = 1;
        resulting_tm->matrix[3][3] = 1;
        for (int i = tms.size()-1; i >=0 ; i--)
        {
            *resulting_tm =   (*resulting_tm) * (*tms.at(i));
        }
        



        child = element->FirstChildElement("Indices");
        std::string indices_text = child->GetText(); // Get the indices text as a string

        // Clear the stream and load the new string
        stream.str(indices_text); // Set the string in the stream
        stream.clear();           // Clear the stream state
        
        // Extract the three vertex indices from the stream
        stream >> trianglev1 >> trianglev2 >> trianglev3;


    Vec3 triv1(vertices.at(trianglev1-1).x,vertices.at(trianglev1-1).y,vertices.at(trianglev1-1).z);
    Vec3 triv2(vertices.at(trianglev2-1).x,vertices.at(trianglev2-1).y,vertices.at(trianglev2-1).z);
    Vec3 triv3(vertices.at(trianglev3-1).x,vertices.at(trianglev3-1).y,vertices.at(trianglev3-1).z);
    
    triangles.push_back(new Triangle(
        materials.at(triangle_material_id-1), ObjectType::TriangleType, triv1, triv2, triv3, resulting_tm, nullptr
    ));
    element = element->NextSiblingElement("Triangle");
}
stream.str("");
stream.clear();


// //Get Spheres
element = root->FirstChildElement("Objects");
element = element->FirstChildElement("Sphere");
while (element)
{
    child = element->FirstChildElement("Material");
    stream << child->GetText() << std::endl;
    stream >> sphere_material_id;

    child = element->FirstChildElement("Center");
    stream << child->GetText() << std::endl;
    stream >> sphere_center_id;

    child = element->FirstChildElement("Radius");
    stream << child->GetText() << std::endl;
    stream >> shpere_radius;

    std::vector<TransformationMatrix*> tms;
    child = element->FirstChildElement("Transformations");
    if(child){
        stream.clear();
        stream << child->GetText() << std::endl;
        string transformation_buffer;
        while(stream >> transformation_buffer){
            char op = transformation_buffer.at(0);
            int id = std::stoi(transformation_buffer.substr(1));
            if(op=='r'){
                tms.push_back(new TransformationMatrix(rotate.at(id-1), op));
            }else if (op=='t')
            {
                tms.push_back(new TransformationMatrix(translate.at(id-1), op));
            }else if (op=='s')
            {
                tms.push_back(new TransformationMatrix(scale.at(id-1), op));
            }
        }
    }
    // Initialize resulting tm as identity matrix so that it means no transformation
    TransformationMatrix* resulting_tm = new TransformationMatrix();
    resulting_tm->matrix[0][0] = 1;
    resulting_tm->matrix[1][1] = 1;
    resulting_tm->matrix[2][2] = 1;
    resulting_tm->matrix[3][3] = 1;
    for (int i = 0; i < tms.size(); i++)
    {
        *resulting_tm = *resulting_tm * (*tms.at( tms.size() - i - 1));
    }
    

    Sphere* s = new Sphere(
    vertices.at(sphere_center_id-1),
    shpere_radius,  
    materials.at(sphere_material_id-1),
    ObjectType::SphereType, resulting_tm);
    element = element->NextSiblingElement("Sphere");
    spheres.push_back(s);
}








std::vector<Object*> all_objects;
// Now that we have read the file we can convert it to our format
int num_objects = meshes.size() + spheres.size() + triangles.size();

for (size_t i = 0; i < meshes.size(); i++)
{
    all_objects.push_back(meshes.at(i));
}
for (size_t i = 0; i < spheres.size(); i++)
{
    all_objects.push_back(spheres.at(i));
}
for (size_t i = 0; i < triangles.size(); i++)
{
    all_objects.push_back(triangles.at(i));
}



// Get Mesh Instances
element = root->FirstChildElement("Objects");
element = element->FirstChildElement("MeshInstance");
while(element){
    float parent_id;
    bool reset_transform = false;



    child = element->FirstChildElement("Material");
    stream.clear();
    string tmp = child->GetText();
    stream << tmp << std::endl;
    stream >> mesh_material_id;
    mesh_material = materials.at(mesh_material_id-1);

    
    std::vector<TransformationMatrix*> tms;
    child = element->FirstChildElement("Transformations");
    if(child){
        string tmp = child->GetText();
        stream << tmp << std::endl;
        string transformation_buffer;
        while(stream >> transformation_buffer){
            char op = transformation_buffer.at(0);
            int id = std::stoi(transformation_buffer.substr(1));
            if(op=='r'){
                tms.push_back(new TransformationMatrix(rotate.at(id-1), op));
            }else if (op=='t')
            {
                tms.push_back(new TransformationMatrix(translate.at(id-1), op));
            }else if (op=='s')
            {
                tms.push_back(new TransformationMatrix(scale.at(id-1), op));
            }
        }
    }
     // Initialize resulting tm as identity matrix so that it means no transformation
    TransformationMatrix* resulting_tm = new TransformationMatrix();
    resulting_tm->matrix[0][0] = 1;
    resulting_tm->matrix[1][1] = 1;
    resulting_tm->matrix[2][2] = 1;
    resulting_tm->matrix[3][3] = 1;
    for (int i = 0; i < tms.size(); i++)
    {
        *resulting_tm = *resulting_tm * (*tms.at( tms.size() - i - 1));
    }

    
    stream.clear();
    stream.str("");

    stream << element->Attribute("baseMeshId", NULL) << std::endl; 
    stream >> parent_id;


    
    string reset = element->Attribute("resetTransform", NULL) ;
    reset_transform = (reset == "true");

    ObjectInstance* new_obj  = new ObjectInstance(meshes.at(parent_id-1), reset_transform, resulting_tm, materials.at(mesh_material_id-1));
    meshes.push_back(new_obj);
    all_objects.push_back(new_obj);
    element = element->NextSiblingElement("MeshInstance");
}










num_objects = all_objects.size();
Object** objlist = new Object*[num_objects];
for (size_t i = 0; i < all_objects.size(); i++)
{
    objlist[i] = all_objects.at(i);
    objlist[i]->id = i;
}



std::vector<Scene*> scenes;
// for each camera we need to define a image pane
for (size_t i = 0; i < cameras.size(); i++)
{
    
    ImagePane* imagePane = new ImagePane(
        img_height , img_width, cam_near_plane.x, cam_near_plane.y,  cam_near_plane.z, cam_near_plane.h, cameras.at(i)->mint , cameras.at(i)
    );
    Light** lights_array = new Light*[lights.size()];
    for (size_t i = 0; i < lights.size(); i++)
    {
        lights_array[i] = lights.at(i);
    }
    Vec3 bg(background_color.x,background_color.y,background_color.z);
    Vec3 ambli(ambient_light.x, ambient_light.y, ambient_light.z);
    // TODO refraction index not give thus leaving none
    Scene* s = new Scene(
        objlist, 
        num_objects,
        cameras.at(i), 
        imagePane, 
        lights_array, 
        lights.size(), 
        bg, 
        ambli, shadow_ray_eps, 1
    );
    scenes.push_back(s);
}

return scenes;


//TODO  burayi desgistir her kamera icin sahne tanimlamana gerek yok


}