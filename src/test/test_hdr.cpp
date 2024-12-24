// #include <iostream>
// #include <vector>
// #include <tinyexr.h>
// #include <cstring>  // For memset
// #include "lodepng.h"



// template <typename T>
// T clamp(T val, T minVal, T maxVal) {
//     if (val < minVal) return minVal;
//     if (val > maxVal) return maxVal;
//     return val;
// }


// int main( int argc, char* argv[]) {
//   float* image; // width * height * RGBA
//   int width;
//   int height;
//   const char* err = NULL; // or nullptr in C++11

//   int ret = LoadEXR(&image, &width, &height, argv[1], &err);

//   if (ret != TINYEXR_SUCCESS) {
//     if (err) {
//        fprintf(stderr, "ERR : %s\n", err);
//        FreeEXRErrorMessage(err); // release memory of error message.
//     }
//   } else {


//     std::vector<unsigned char> image_mat(width * height * 4, 255); // White image
//     int i = 0;
//     // Set some pixels to black
//     for (int x = 0; x < width; ++x) {
//         for (int y = 0; y < height; ++y) {
//             int idx = 4 * (y * width + x);
//             int idx1 = 4 * (y * width + x);
//             // printf("pixel: %f\n", clamp(image[idx1+0]*255, 0.0f, 255.0f));
//             image_mat[idx + 0] = static_cast<uint8_t>(clamp(image[idx1+0]*255, 0.0f, 255.0f));
//             image_mat[idx + 1] = static_cast<uint8_t>(clamp(image[idx1+1]*255, 0.0f, 255.0f));
//             image_mat[idx + 2] = static_cast<uint8_t>(clamp(image[idx1+2]*255, 0.0f, 255.0f));
//             image_mat[idx + 3] = 255; // Alpha (opaque)
//         }
//     }

//     // Encode the image and save to file
//     unsigned error = lodepng::encode("Alper.png", image_mat, width, height);


//     free(image); // release memory of image data
//   }
// }