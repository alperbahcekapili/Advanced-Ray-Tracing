#pragma once
#include <stdio.h>
#include <string>
#include "../util/data_structures.h"

enum InterploationType {
    BILINEAR,
    NEAREAST_NEIGHBOR
};

class TextureImage {
    private:
    public: 
        std::string path;
        int width;
        int height;
        int channels;
        unsigned char* data;
        TextureImage (std::string path);
        ~TextureImage();
        Vec3 get_value(float u, float v, InterploationType type);
        
};