#include <stdio.h>
#include "TextureImage.h"
#include "../util/lodepng.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../util/stbimage.h"
#include "../util/tinyexr.h"
#include "../util/util.h"

TextureImage::TextureImage(std::string path, bool hdr){
    this->path = path;
    // read this image into memory and be ready for querying
    int width, height, channels;

    if (!hdr)
    {// Load the image
        auto data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
        }

        std::cout << "Image loaded successfully: "
                    << width << " x " << height
                    << " with " << channels << " channels."
                    << std::endl;

        this->width = width;
        this->height = height;
        this->channels = channels;
        this->data = new float[width * height * channels];

        for (int i = 0; i < width * height * channels; ++i) {
            this->data[i] = data[i];
        }

        

    }
    else{

        float* image; // width * height * RGBA
        int width;
        int height;
        const char* err = NULL; // or nullptr in C++11

        int ret = LoadEXR(&image, &width, &height, path.c_str(), &err);
        
        this->data = image;
        this->channels = 4;
        this->height = height;
        this->width = width;

    }

}

TextureImage::~TextureImage(){

}


Vec3 TextureImage::get_value(float u, float v, InterploationType type){

    u = abs(u);
    v = abs(v);

    if(type == NEAREAST_NEIGHBOR){
        int x = int(u * this->width);
        int y = int(v * this->height);
        int pixel_offset = (y * this->width + x) * this->channels;
        float r = data[pixel_offset] / 255.0f;
        float g = data[pixel_offset+1] / 255.0f;
        float b = data[pixel_offset+2] / 255.0f;
        return Vec3(r, g, b);
    }else if (type == BILINEAR)
    {
        u = abs(u  - int(u));
        v = abs(v  - int(v));
        

        // Calculate the coordinates of the top-left corner of the 2x2 square surrounding the desired pixel
        float x1 = floor(u * this->width);
        float y1 = floor(v * this->height);
        float x2 = std::min(x1 + 1, float(this->width - 1)); // Ensure x2 is within bounds
        float y2 = std::min(y1 + 1, float(this->height - 1)); // Ensure y2 is within bounds

        // Calculate the distances from the four surrounding pixels to the point (u, v)
        float dx = (u * this->width) - x1;
        float dy = (v * this->height) - y1;

        // Get the pixel values at the four corners
        int pixel_offset_1 = (int(y1) * this->width + int(x1)) * this->channels;
        int pixel_offset_2 = (int(y1) * this->width + int(x2)) * this->channels;
        int pixel_offset_3 = (int(y2) * this->width + int(x1)) * this->channels;
        int pixel_offset_4 = (int(y2) * this->width + int(x2)) * this->channels;

        // Extract the colors of the four pixels
        float r1 = data[pixel_offset_1] / 255.0f;
        float g1 = data[pixel_offset_1 + 1] / 255.0f;
        float b1 = data[pixel_offset_1 + 2] / 255.0f;

        float r2 = data[pixel_offset_2] / 255.0f;
        float g2 = data[pixel_offset_2 + 1] / 255.0f;
        float b2 = data[pixel_offset_2 + 2] / 255.0f;

        float r3 = data[pixel_offset_3] / 255.0f;
        float g3 = data[pixel_offset_3 + 1] / 255.0f;
        float b3 = data[pixel_offset_3 + 2] / 255.0f;

        float r4 = data[pixel_offset_4] / 255.0f;
        float g4 = data[pixel_offset_4 + 1] / 255.0f;
        float b4 = data[pixel_offset_4 + 2] / 255.0f;

        // Perform bilinear interpolation on each color channel
        float r = (1 - dx) * (1 - dy) * r1 + dx * (1 - dy) * r2 + (1 - dx) * dy * r3 + dx * dy * r4;
        float g = (1 - dx) * (1 - dy) * g1 + dx * (1 - dy) * g2 + (1 - dx) * dy * g3 + dx * dy * g4;
        float b = (1 - dx) * (1 - dy) * b1 + dx * (1 - dy) * b2 + (1 - dx) * dy * b3 + dx * dy * b4;

        return Vec3(r, g, b);
    }
    
}