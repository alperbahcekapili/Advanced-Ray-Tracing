

#include <vector>
#include <cassert>
#include "../models/Ray.h"
#include "../models/Object.h"
#include "../models/Ray.h"
#include "../lights/Light.h"
#include "../lights/DirectionalLight.h"
#include "../lights/SphericalDirectionalLight.h"
#include "util.h"
#include "tinyexr.h"
#include <cstring>
#include <math.h>

using namespace std;

#define assertm(exp, msg) assert(((void)msg, exp))



  // See `examples/rgbe2exr/` for more details.
  bool SaveEXR(const float* rgb, int width, int height, const char* outfilename) {

    EXRHeader header;
    InitEXRHeader(&header);

    EXRImage image;
    InitEXRImage(&image);

    image.num_channels = 3;

    std::vector<float> images[3];
    images[0].resize(width * height);
    images[1].resize(width * height);
    images[2].resize(width * height);

    // Split RGBRGBRGB... into R, G and B layer
    for (int i = 0; i < width * height; i++) {
      images[0][i] = rgb[3*i+0];
      images[1][i] = rgb[3*i+1];
      images[2][i] = rgb[3*i+2];
    }

    float* image_ptr[3];
    image_ptr[0] = &(images[2].at(0)); // B
    image_ptr[1] = &(images[1].at(0)); // G
    image_ptr[2] = &(images[0].at(0)); // R

    image.images = (unsigned char**)image_ptr;
    image.width = width;
    image.height = height;

    header.num_channels = 3;
    header.channels = (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels);
    // Must be (A)BGR order, since most of EXR viewers expect this channel order.
    strncpy(header.channels[0].name, "B", 255); header.channels[0].name[std::strlen("B")] = '\0';
    strncpy(header.channels[1].name, "G", 255); header.channels[1].name[std::strlen("G")] = '\0';
    strncpy(header.channels[2].name, "R", 255); header.channels[2].name[std::strlen("R")] = '\0';

    header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
    header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
    for (int i = 0; i < header.num_channels; i++) {
      header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
      header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of output image to be stored in .EXR
    }

    const char* err = NULL; // or nullptr in C++11 or later.
    int ret = SaveEXRImageToFile(&image, &header, outfilename, &err);
    if (ret != TINYEXR_SUCCESS) {
      fprintf(stderr, "Save EXR err: %s\n", err);
      FreeEXRErrorMessage(err); // free's buffer for an error message
      return ret;
    }
    printf("Saved exr file. [ %s ] \n", outfilename);

    // free(rgb);

    free(header.channels);
    free(header.pixel_types);
    free(header.requested_pixel_types);

  }

  

// Degamma correction for a single color
Vec3 degammaCorrectColor(const Vec3& color, float gamma) {
    // Scale each channel back to [0, 1]
    float scaledR = color.x / 255.0f;
    float scaledG = color.y / 255.0f;
    float scaledB = color.z / 255.0f;

    // Apply inverse gamma correction
    Vec3 correctedColor;
    correctedColor.x = std::pow(scaledR, gamma);
    correctedColor.y = std::pow(scaledG, gamma);
    correctedColor.z = std::pow(scaledB, gamma);

    return correctedColor;
}
float degammaCorrectColor(float& color, float gamma){
    float scaledR = color / 255.0f;
    return std::pow(scaledR, gamma);
}

float gammaCorrectColor(float& color, float gamma){
    float clampedR = std::clamp(color, 0.0f, 1.0f);
    return 255.0f * std::pow(clampedR, 1.0f / gamma);
}


void gammaCorrectImage(float* image, int channels, int width, int height, float gamma){
    for (int i = 0; i < height * width * channels; ++i) {
        image[i] = gammaCorrectColor(image[i], gamma);
    }
        
}

// Degamma correction for an entire image
void degammaCorrectImage(Vec3** image, int width, int height, float gamma) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = degammaCorrectColor(image[i][j], gamma);
        }
    }
}


void degammaCorrectImage(float* image, int channels, int width, int height, float gamma){
    for (int i = 0; i < height * width * channels; ++i) {
        image[i] = degammaCorrectColor(image[i], gamma);
    }
        
}




#include <algorithm>
#include <limits>

void scale_floats_0_1(float* data, size_t size) {
    // Find the minimum and maximum values in the array
    float min_val = *std::min_element(data, data + size);
    float max_val = *std::max_element(data, data + size);

    // Handle the case where all values are the same
    if (min_val == max_val) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = 0.0f;
        }
        return;
    }

    // Scale the values to the range [0, 1]
    for (size_t i = 0; i < size; ++i) {
        data[i] = (data[i] - min_val) / (max_val - min_val);
    }
}


// Gamma correction for a single color
Vec3 gammaCorrectColor(const Vec3& color, float gamma) {
    // Clamp each channel to the [0, 1] range
    float clampedR = std::clamp(color.x, 0.0f, 1.0f);
    float clampedG = std::clamp(color.y, 0.0f, 1.0f);
    float clampedB = std::clamp(color.z, 0.0f, 1.0f);

    // Apply gamma correction
    Vec3 correctedColor;
    correctedColor.x = 255.0f * std::pow(clampedR, 1.0f / gamma);
    correctedColor.y = 255.0f * std::pow(clampedG, 1.0f / gamma);
    correctedColor.z = 255.0f * std::pow(clampedB, 1.0f / gamma);

    return correctedColor;
}

// Gamma correction for an entire image
void gammaCorrectImage(Vec3** image, int width, int height, float gamma) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = gammaCorrectColor(image[i][j], gamma);
        }
    }
}


void convertVec3ToFlatArray(Vec3** image, int width, int height, float* flatArray) {
    int index = 0;
    for (int j = 0; j < height ; ++j)
    {
        for (int i = 0; i < width; ++i)
        
        {
            Vec3 pixel = image[i][j];
            // printf("%f ", pixel.x);
            // printf("%f ", pixel.y);
            // printf("%f \n", pixel.z);
            flatArray[index++] = pixel.x;
            flatArray[index++] = pixel.y;
            flatArray[index++] = pixel.z;
        }
    }
}


void reinhardGlobalTonemap(Vec3** image, int width, int height, float burnout, float saturation, float alpha) {
    const float epsilon = 1e-6;

    // Step 1: Compute luminance
    std::vector<float> luminanceValues;
    luminanceValues.reserve(width * height);

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height ; ++y) {
            Vec3 pixel = image[x][y];
            float luminance = 0.2126f * pixel.x + 0.7152f * pixel.y + 0.0722f * pixel.z;
            luminanceValues.push_back(luminance);
        }
    }

    // Step 2: Compute the logarithmic average luminance
    float logSum = 0.0;
    for (float luminance : luminanceValues) {
        logSum += std::log(luminance + epsilon);
    }
    float logAvgLuminance = std::exp(logSum / luminanceValues.size()) ;

    
    std::vector<float> sortedNumbers = luminanceValues;
    std::sort(sortedNumbers.begin(), sortedNumbers.end());
    size_t index = static_cast<size_t>(sortedNumbers.size() * ((100.0f - burnout)/100.0f)) - 1;  // -1 because vector indexing starts from 0
    float Lwhite = sortedNumbers[index];


    // Step 3: Scale the luminance
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height ; ++y) {
            Vec3& pixel = image[x][y];
            float luminance = 0.2126f * pixel.x + 0.7152f * pixel.y + 0.0722f * pixel.z;
            float scaledLuminance = (alpha / logAvgLuminance) * luminance;

            

            // Step 4: Apply tone mapping with burnout parameter
            float mappedLuminance = (scaledLuminance * (1.0f + (scaledLuminance / (Lwhite * Lwhite)))) / (1.0f + scaledLuminance);


            pixel.x = mappedLuminance * pow(pixel.x / luminance, saturation);
            pixel.y = mappedLuminance * pow(pixel.y / luminance, saturation);
            pixel.z = mappedLuminance * pow(pixel.z / luminance, saturation);

        }
    }

}






float det3x3(float m[3][3]){
    // satir, sutun
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[1][0] * (m[0][2] * m[2][1] - m[0][1] * m[2][2]) + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
}

Vec3 clipValues(Vec3 v, float clip_val){
    if(v.x > clip_val)
        v.x  = clip_val;
    if(v.y  > clip_val)
        v.y  = clip_val;
    if(v.z > clip_val)
        v.z = clip_val;
    return v;
    
}


Ray createRayFrom(Vec3 start, Vec3 destination){
    // In order to create ray from start and destination we need to substract start from destination
    Vec3 scaledRay = destination - start;
    return Ray(start, scaledRay);
}


Ray createLightRay(Light* light, Vec3 destination){
    if(light->ltype == DirectionalLightType){
        DirectionalLight* dir_light = dynamic_cast<DirectionalLight*>(light);
        Vec3 dir = dir_light->getDirection();
        Vec3 origin = destination - (99999 * dir); // light comes from out of the scene
        return Ray(origin, dir);
    }
    else if (light->ltype == SphericalDirectionalLightType)
    {
        SphericalDirectionalLight* sph_light = dynamic_cast<SphericalDirectionalLight*>(light);
        Vec3 randomdir  = Vec3(generate_random_01(), generate_random_01(), generate_random_01());
        return Ray(Vec3(0,0,0), randomdir);
    }
    
    Vec3 start = light->getPointOn();
    return createRayFrom(start, destination);
}


void printObjectList(Object** l, int len){
    for (size_t i = 0; i < len; i++)
    {
        std::cout << i << ": " <<l[i]->getCenter().x << ", " << l[i]->getCenter().y << ", " << l[i]->getCenter().z << "\n";
    }
    std::cout << "\n\n";
    
}