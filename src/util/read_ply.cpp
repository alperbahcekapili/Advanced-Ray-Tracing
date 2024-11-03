#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <sstream>  // Include this header for istringstream
#include "read_ply.h"
#include "data_structures.h"

std::vector<Vec3> read_ply(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    

    std::vector<Vec3> vertices;
    std::vector<Vec3> faces;

    std::string line;
    int num_vertices = 0, num_faces = 0;

    bool binary = true;

    // Read header lines until "end_header"
    while (std::getline(file, line)) {
        if(line.find("format ascii") != std::string::npos)
            binary = false;

        if (line.find("end_header") != std::string::npos) {
            break;
        }
        if (line.find("element vertex") != std::string::npos) {
            std::istringstream iss(line);
            std::string word;
            iss >> word >> word >> num_vertices;
        }
        if (line.find("element face") != std::string::npos) {
            std::istringstream iss(line);
            std::string word;
            iss >> word >> word >> num_faces;
        }
    }



    if(binary){

    // Read vertex data
    for (int i = 0; i < num_vertices; ++i) {
        Vec3 vertex;
        file.read(reinterpret_cast<char*>(&vertex.x), sizeof(vertex.x));
        file.read(reinterpret_cast<char*>(&vertex.y), sizeof(vertex.y));
        file.read(reinterpret_cast<char*>(&vertex.z), sizeof(vertex.z));

      
            // No color, just store the coordinates
            vertices.push_back(vertex);
        
    }

    // Read face data
    for (int i = 0; i < num_faces; ++i) {
        Face face;
        
        int index;
        u_char numelems;
        file.read(reinterpret_cast<char*>(&numelems), sizeof(numelems));  // Read each index
        face.vertex_indices.resize(numelems);  // Resize the vector to hold 3 indices
        for (int i = 0; i < numelems; i++)
        {
            file.read(reinterpret_cast<char*>(&index), sizeof(index));  // Read each index    
            if(i>2){
                faces.push_back(vertices.at(face.vertex_indices[0]));
                faces.push_back(vertices.at(face.vertex_indices[i-1]));
            }
            face.vertex_indices[i] = index;  // Store the index at the correct position
            faces.push_back(vertices.at(index)); // TODO: Here if verte
        }
        
    }

    file.close();

    }else{
        
    // Now read the elements
    int vertex_count = 0;
    int face_count = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        if (vertex_count < num_vertices) {
            Vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            
            ++vertex_count;
        }
        else if (face_count < num_faces) {
            Face face;
            iss >> face.num_vertices;
            face.vertex_indices.resize(face.num_vertices);  // Resize the vector to hold 3 indices
            for (int i = 0; i < face.num_vertices; ++i) {
                int index;
                iss >> index;
                if(i>2){
                    faces.push_back(vertices.at(face.vertex_indices[0]));
                    faces.push_back(vertices.at(face.vertex_indices[i-1]));
                }
                face.vertex_indices[i] = index;  // Store the index at the correct position
                faces.push_back(vertices.at(index)); // TODO: Here if verte
            }
            
            ++face_count;
        }
        
    }


    file.close();
    }


    for (size_t i = 0; i < faces.size(); i++)
    {
        std::cout << faces.at(i).x << ", " << faces.at(i).y << ", " << faces.at(i).z << std::endl;
    }
    
    


    // Now you can work with vertices and faces
    std::cout << "Read " << vertices.size() << " vertices and " << faces.size() << " faces." << std::endl;

    return faces;
}
