#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <sstream>  // Include this header for istringstream
#include "data_structures.h"

struct Vertex {
    float x, y, z;  // Vertex coordinates
};

struct Face {
    int num_vertices;
    std::vector<int> vertex_indices;
};

std::vector<Vec3> read_ply(const std::string& filename);