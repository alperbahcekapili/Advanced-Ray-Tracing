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
#include "../scene/Scene.h"


std::vector<Scene*> loadFromXml(const std::string &filepath)
{
    tinyxml2::XMLDocument file;
    std::stringstream stream;
    
    auto res = file.LoadFile(filepath.c_str());
    float shadow_ray_eps = 0;
    std::vector<int> background_color = {0,0,0};
    int max_recursion_depth = 0;

    std::vector<float> cam_position = {0,0,0};
    std::vector<float> cam_gaze = {0,0,0};
    std::vector<float> cam_up = {0,0,0};
    std::vector<float> cam_near_plane = {0,0,0,0};
    float cam_near_distance = 0;
    int img_width;
    int img_height;
    std::string camera_name;
    std::vector<Camera*> cameras;


    std::vector<int> ambient_light = {0,0,0};
    std::vector<PointLight*> lights; 
    std::vector<float> light_position = {0,0,0};
    std::vector<float> light_intensity = {0,0,0};



    std::vector<Material*> materials;
    MaterialType material_type;
    std::vector<float> ambient_reflectance = {0,0,0};
    std::vector<float> diffuse_reflectance = {0,0,0};
    std::vector<float> specular_reflectance = {0,0,0};
    std::vector<float> mirror_reflectance = {0,0,0};
    std::vector<float> absorption_coefficent = {0,0,0};
    float absorption_index;
    bool is_mirror;
    float phong_exponent;
    float refraction_index;


    std::vector<std::vector<float>> vertices;
    std::vector<float> cur_vertex = {1,1,1};


    std::vector<int> mesh_vertex_ids = {0,0,0};
    Material* mesh_material;
    int mesh_material_id;
    int facevid1, facevid2, facevid3;
    std::vector<std::array<std::array<float, 3>, 3>> mesh_faces;
    int mesh_numfaces = 0;
    std::vector<Mesh*> meshes;


    int triangle_material_id;
    int trianglev1, trianglev2, trianglev3;
    std::vector<Triangle*> triangles;


    int sphere_material_id;
    float shpere_radius;
    int sphere_center_id;
    std::vector<Sphere*> spheres;


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
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0 0 0" << std::endl;
    }
    stream >> background_color.at(0) >> background_color.at(1) >> background_color.at(2);
    

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
        auto child = element->FirstChildElement("Position");
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

        stream >>  cam_position.at(0) >> cam_position.at(1) >> cam_position.at(2);
        stream >> cam_gaze.at(0) >> cam_gaze.at(1) >> cam_gaze.at(2);
        stream >> cam_up.at(0) >> cam_up.at(1) >> cam_up.at(2);
        stream >> cam_near_plane.at(0) >> cam_near_plane.at(1) >> cam_near_plane.at(2) >> cam_near_plane.at(3);
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
    auto child = element->FirstChildElement("AmbientLight");
    stream << child->GetText() << std::endl;
    stream >> ambient_light.at(0) >> ambient_light.at(1) >> ambient_light.at(2);
    element = element->FirstChildElement("PointLight");


    while (element)
    {
        child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Intensity");
        stream << child->GetText() << std::endl;

        stream >> light_position.at(0) >> light_position.at(1) >> light_position.at(2);
        stream >> light_intensity.at(0) >> light_intensity.at(1) >> light_intensity.at(2);
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


        stream >> ambient_reflectance.at(0) >> ambient_reflectance.at(1) >> ambient_reflectance.at(2);
        stream >> diffuse_reflectance.at(0) >> diffuse_reflectance.at(1) >> diffuse_reflectance.at(2);
        stream >> specular_reflectance.at(0) >>  specular_reflectance.at(1) >>  specular_reflectance.at(2);
        stream >> refraction_index;
        stream >> absorption_coefficent.at(0) >>  absorption_coefficent.at(1) >>  absorption_coefficent.at(2);
        stream >> absorption_index;
        stream >> mirror_reflectance.at(0) >> mirror_reflectance.at(1)>> mirror_reflectance.at(2);
        
        stream >> phong_exponent;
        Material* material = new Material(material_type, ambient_reflectance, diffuse_reflectance, specular_reflectance, phong_exponent, mirror_reflectance, refraction_index, absorption_index, absorption_coefficent);
        materials.push_back(material);
        element = element->NextSiblingElement("Material");
    }
    stream.clear();  // Clear any potential error state flags
    stream.str(""); // Clear the stream content


    element = root->FirstChildElement("VertexData");
    stream << element->GetText() << std::endl;
    
    while (stream >> cur_vertex.at(0) >> cur_vertex.at(1) >> cur_vertex.at(2))
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
        stream << child->GetText() << std::endl;
        stream >> mesh_material_id;
        mesh_material = materials.at(mesh_material_id-1);

        child = element->FirstChildElement("Faces");
        stream << child->GetText() << std::endl;
        
        
        while (!(stream >> facevid1).eof())
        {
            stream >> facevid2 >> facevid3;
            std::array<std::array<float, 3>, 3>  mesh_face = {{
                {vertices.at(facevid1-1).at(0),vertices.at(facevid1-1).at(1),vertices.at(facevid1-1).at(2)},
                {vertices.at(facevid2-1).at(0),vertices.at(facevid2-1).at(1),vertices.at(facevid2-1).at(2)},
                {vertices.at(facevid3-1).at(0),vertices.at(facevid3-1).at(1),vertices.at(facevid3-1).at(2)}
            }};
            mesh_faces.push_back(mesh_face);
            mesh_numfaces ++;
        }

        stream.clear();
        Mesh* m = new Mesh(materials.at(mesh_material_id-1), ObjectType::MeshType, mesh_faces, mesh_numfaces);
        mesh_numfaces = 0;
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
            std::string material_text = child->GetText(); // Get the material text as a string
        
            // Clear the stream and load the new string to convert to an integer
            stream.str(material_text); // Set the string in the stream
            stream.clear();            // Clear the stream state to avoid errors
            
            // Extract the integer material ID from the stream
            stream >> triangle_material_id;

            child = element->FirstChildElement("Indices");
            std::string indices_text = child->GetText(); // Get the indices text as a string

            // Clear the stream and load the new string
            stream.str(indices_text); // Set the string in the stream
            stream.clear();           // Clear the stream state
            
            // Extract the three vertex indices from the stream
            stream >> trianglev1 >> trianglev2 >> trianglev3;


        float triv1[3] = {vertices.at(trianglev1-1).at(0),vertices.at(trianglev1-1).at(1),vertices.at(trianglev1-1).at(2)};
        float triv2[3] = {vertices.at(trianglev2-1).at(0),vertices.at(trianglev2-1).at(1),vertices.at(trianglev2-1).at(2)};
        float triv3[3] = {vertices.at(trianglev3-1).at(0),vertices.at(trianglev3-1).at(1),vertices.at(trianglev3-1).at(2)};
        
        triangles.push_back(new Triangle(
            materials.at(triangle_material_id-1), ObjectType::TriangleType, triv1, triv2, triv3
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

        Sphere* s = new Sphere(
        vertices.at(sphere_center_id-1).at(0),
        vertices.at(sphere_center_id-1).at(1),
        vertices.at(sphere_center_id-1).at(2),
        shpere_radius,  
        materials.at(sphere_material_id-1),
        ObjectType::SphereType);
        element = element->NextSiblingElement("Sphere");
        spheres.push_back(s);
    }








    // Now that we have read the file we can convert it to our format
    int num_objects = meshes.size() + spheres.size() + triangles.size();
    Object** objlist = new Object*[num_objects];
    for (size_t i = 0; i < meshes.size(); i++)
    {
        objlist[i] = meshes.at(i);
    }
    for (size_t i = 0; i < spheres.size(); i++)
    {
        objlist[i+meshes.size()] = spheres.at(i);
    }
    for (size_t i = 0; i < triangles.size(); i++)
    {
        objlist[i+meshes.size()+spheres.size()] = triangles.at(i);
    }

    std::vector<Scene*> scenes;
    // for each camera we need to define a image pane
    for (size_t i = 0; i < cameras.size(); i++)
    {
        
        ImagePane* imagePane = new ImagePane(
            img_height , img_width, cam_near_plane.at(0), cam_near_plane.at(1),  cam_near_plane.at(2), cam_near_plane.at(3), cameras.at(i)->mint , cameras.at(i)
        );
        Light** lights_array = new Light*[lights.size()];
        for (size_t i = 0; i < lights.size(); i++)
        {
            lights_array[i] = lights.at(i);
        }
        int bg[3] = {background_color.at(0),background_color.at(1),background_color.at(2)};
        int ambli[3] = {ambient_light.at(0), ambient_light.at(1), ambient_light.at(2)};
        // TODO refraction index not give thus leaving none
        Scene* s = new Scene(
            objlist, 
            meshes.size()+spheres.size()+triangles.size(),
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
