#pragma once
#include <cmath>
#include <array>
#include <iostream>
#include <algorithm>


struct Vec3 {
    float x, y, z;

    // Constructor
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}


    // Calculate the centroid
    static Vec3 centroid(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
        return Vec3{
            (v1.x + v2.x + v3.x) / 3,
            (v1.y + v2.y + v3.y) / 3,
            (v1.z + v2.z + v3.z) / 3
        };
    }

    // Calculate the signed angle from the centroid for sorting
    static float signedAngle(const Vec3& a, const Vec3& b, const Vec3& normal) {
        Vec3 ab = b - a;
        Vec3 refVec{1.0f, 0.0f, 0.0f};  // Reference vector in x direction
        float angle = std::atan2(ab.cross(refVec).z, ab.x);  // Get the angle in the xy plane

        // Adjust angle based on normal direction for clockwise ordering
        if (normal.z < 0) {
            angle = -angle;
        }

        return angle;
    }

    // Sort vertices in clockwise order
    static void sortVerticesClockwise(Vec3& v1, Vec3& v2, Vec3& v3) {
        Vec3 center = centroid(v1, v2, v3);
        Vec3 normal = (v2 - v1).cross(v3 - v1);  // Calculate the normal for orientation

        // Sort vertices based on signed angle with respect to the centroid and normal
        std::array<Vec3, 3> vertices = {v1, v2, v3};
        std::sort(vertices.begin(), vertices.end(), [&](const Vec3& a, const Vec3& b) {
            return signedAngle(center, a, normal) < signedAngle(center, b, normal);
        });

        // Update v1, v2, v3 with sorted vertices
        v1 = vertices[0];
        v2 = vertices[1];
        v3 = vertices[2];
    }

    // Addition operator
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }


    Vec3 operator/(const Vec3& other) const {
        return Vec3(x / other.x, y / other.y, z / other.z);
    }
    // Scalar multiplication (scaling)
    Vec3 operator/(float scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    // Overload for scalar * Vec3 (enables float * Vec3 syntax)
    friend Vec3 operator/(float scalar, const Vec3& vec) {
        return Vec3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
    }


    // Subtraction operator
    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    // Dot product
    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product
    Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Magnitude
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Normalize
    Vec3 normalize() const {
        float mag = magnitude();
        return (mag != 0) ? Vec3(x / mag, y / mag, z / mag) : Vec3(0, 0, 0);
    }

    // Element-wise multiplication
    Vec3 operator*(const Vec3& other) const {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }

    // Scalar multiplication (scaling)
    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    // Overload for scalar * Vec3 (enables float * Vec3 syntax)
    friend Vec3 operator*(float scalar, const Vec3& vec) {
        return Vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

};



struct Vec4 {
    float x, y, z, h;
    Vec4(float x = 0, float y = 0, float z = 0, float h = 0) : x(x), y(y), z(z), h(h) {}
};


struct TransformationMatrix {
    float matrix[4][4];

    // Constructor to initialize the matrix
    TransformationMatrix() {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                matrix[i][j] = 0.0f;
    }

    TransformationMatrix(const Vec3 vec, char type) {
        
        if(type == 's')
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    if(i == 0 && j == 0)
                        matrix[i][j] = vec.x;
                    else if(i == 1 && j == 1)
                        matrix[i][j] = vec.y;
                    else if(i == 2 && j == 2)
                        matrix[i][j] = vec.z;
                    else if (i == 3 && j == 3)
                        matrix[i][j] = 1;
                    else
                        matrix[i][j] = 0;
        else if (type == 't')
        {
            // Setting the translation matrix
            matrix[0][0] = 1.0; matrix[0][1] = 0; matrix[0][2] = 0; matrix[0][3] = vec.x;
            matrix[1][0] = 0; matrix[1][1] = 1.0; matrix[1][2] = 0; matrix[1][3] = vec.y;
            matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = 1.0; matrix[2][3] = vec.z; 
            matrix[3][0] = 0; matrix[3][1] = 0; matrix[3][2] = 0; matrix[3][3] = 1.0;
        }else if (type == 'r')
        {
            // Rotation around the x, y, and z axes
            float cosX = cos(vec.x/90);
            float sinX = sin(vec.x/90);
            float cosY = cos(vec.y/90);
            float sinY = sin(vec.y/90);
            float cosZ = cos(vec.z/90);
            float sinZ = sin(vec.z/90);

            // Create individual rotation matrices
            float rotX[4][4] = {
                {1,      0,       0,      0},
                {0, cosX, -sinX,   0},
                {0, sinX,  cosX,   0},
                {0,      0,       0,      1}
            };

            float rotY[4][4] = {
                { cosY, 0, sinY, 0},
                {     0, 1,    0, 0},
                {-sinY, 0, cosY, 0},
                {     0, 0,    0, 1}
            };

            float rotZ[4][4] = {
                {cosZ, -sinZ, 0, 0},
                {sinZ,  cosZ, 0, 0},
                {    0,     0, 1, 0},
                {    0,     0, 0, 1}
            };

            // Combine the rotations: R = Rz * Ry * Rx
            // Start with identity matrix for the final rotation
            float temp[4][4] = {0};
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    temp[i][j] = 0;
                    for (int k = 0; k < 4; ++k) {
                        temp[i][j] += rotZ[i][k] * rotY[k][j];
                    }
                }
            }

            // Now multiply by rotX
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    matrix[i][j] = 0;
                    for (int k = 0; k < 4; ++k) {
                        matrix[i][j] += temp[i][k] * rotX[k][j];
                    }
                }
            }
        }
        
        
    }
    
    Vec3 transform(Vec3 vec) const {
        Vec3 result;

        result.x = matrix[0][0] * vec.x + matrix[0][1] * vec.y + matrix[0][2] * vec.z + matrix[0][3] * 1.0f;
        result.y = matrix[1][0] * vec.x + matrix[1][1] * vec.y + matrix[1][2] * vec.z + matrix[1][3] * 1.0f;
        result.z = matrix[2][0] * vec.x + matrix[2][1] * vec.y + matrix[2][2] * vec.z + matrix[2][3] * 1.0f;

        return result;
    }


    // Function to multiply two matrices
    TransformationMatrix operator*(TransformationMatrix other) {
        TransformationMatrix result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.matrix[i][j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
                }
            }
        }
        return result;
    }

    // Function to add two matrices
    TransformationMatrix operator+( TransformationMatrix other) {
        TransformationMatrix result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
            }
        }
        return result;
    }

    // Function to transpose the matrix
    TransformationMatrix transpose() const {
        TransformationMatrix result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.matrix[j][i] = matrix[i][j];
            }
        }
        return result;
    }

    // Function to compute the determinant of the matrix
    float determinant() const {
        float det = 0.0f;
        for (int i = 0; i < 4; ++i) {
            det += (matrix[0][i] * cofactor(0, i));
        }
        return det;
    }

    // Function to compute the cofactor of an element
    float cofactor(int row, int col) const {
        float minor[3][3];
        int minorRow = 0, minorCol = 0;

        // Fill in the 3x3 minor matrix by skipping the specified `row` and `col`
        for (int i = 0; i < 4; ++i) {
            if (i == row) continue;
            minorCol = 0;
            for (int j = 0; j < 4; ++j) {
                if (j == col) continue;
                minor[minorRow][minorCol] = matrix[i][j];
                minorCol++;
            }
            minorRow++;
        }

        // Calculate the cofactor, applying the correct sign
        return determinant3x3(minor) * ((row + col) % 2 == 0 ? 1 : -1);
    }

    // Function to compute the determinant of a 3x3 matrix
    float determinant3x3(const float mat[3][3]) const {
        return mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
             - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
             + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    }


    
    // Function to compute the inverse of the matrix
    TransformationMatrix inverse() const {
        float det = determinant();
        TransformationMatrix result;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.matrix[j][i] = cofactor(i, j) / det;
            }
        }
        return result;
    }

 TransformationMatrix inverseUpperLeft3x3() const {
    // Create a 3x3 matrix for the upper left portion
    float subMatrix[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            subMatrix[i][j] = matrix[i][j];
        }
    }

    // Calculate the determinant of the 3x3 submatrix
    float det = subMatrix[0][0] * (subMatrix[1][1] * subMatrix[2][2] - subMatrix[1][2] * subMatrix[2][1])
              - subMatrix[0][1] * (subMatrix[1][0] * subMatrix[2][2] - subMatrix[1][2] * subMatrix[2][0])
              + subMatrix[0][2] * (subMatrix[1][0] * subMatrix[2][1] - subMatrix[1][1] * subMatrix[2][0]);

    if (det == 0) {
        throw std::runtime_error("Matrix is singular, cannot invert.");
    }

    // Compute the inverse of the 3x3 submatrix
    float invDet = 1.0f / det;
    float invSubMatrix[3][3];
    invSubMatrix[0][0] =  (subMatrix[1][1] * subMatrix[2][2] - subMatrix[1][2] * subMatrix[2][1]) * invDet;
    invSubMatrix[0][1] = -(subMatrix[0][1] * subMatrix[2][2] - subMatrix[0][2] * subMatrix[2][1]) * invDet;
    invSubMatrix[0][2] =  (subMatrix[0][1] * subMatrix[1][2] - subMatrix[0][2] * subMatrix[1][1]) * invDet;
    invSubMatrix[1][0] = -(subMatrix[1][0] * subMatrix[2][2] - subMatrix[1][2] * subMatrix[2][0]) * invDet;
    invSubMatrix[1][1] =  (subMatrix[0][0] * subMatrix[2][2] - subMatrix[0][2] * subMatrix[2][0]) * invDet;
    invSubMatrix[1][2] = -(subMatrix[0][0] * subMatrix[1][2] - subMatrix[0][2] * subMatrix[1][0]) * invDet;
    invSubMatrix[2][0] =  (subMatrix[1][0] * subMatrix[2][1] - subMatrix[1][1] * subMatrix[2][0]) * invDet;
    invSubMatrix[2][1] = -(subMatrix[0][0] * subMatrix[2][1] - subMatrix[0][1] * subMatrix[2][0]) * invDet;
    invSubMatrix[2][2] =  (subMatrix[0][0] * subMatrix[1][1] - subMatrix[0][1] * subMatrix[1][0]) * invDet;

    // Create the resulting 4x4 transformation matrix
    TransformationMatrix result;

    // Copy the inverse 3x3 submatrix into the upper-left part of the result matrix
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.matrix[i][j] = invSubMatrix[i][j];
        }
    }

    // Set the remaining elements in the 4x4 result matrix
    for (int i = 0; i < 4; ++i) {
        result.matrix[i][3] = 0.0f; // Last column, except the bottom-right
        result.matrix[3][i] = 0.0f; // Last row, except the bottom-right
    }
    result.matrix[3][3] = 1.0f; // Bottom-right element

    return result;
}













    TransformationMatrix inverseTransposeUpperLeft3x3() const {
    // Create a 3x3 matrix for the upper left portion
    float subMatrix[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            subMatrix[i][j] = matrix[i][j];
        }
    }

    // Calculate the determinant of the 3x3 submatrix
    float det = subMatrix[0][0] * (subMatrix[1][1] * subMatrix[2][2] - subMatrix[1][2] * subMatrix[2][1])
              - subMatrix[0][1] * (subMatrix[1][0] * subMatrix[2][2] - subMatrix[1][2] * subMatrix[2][0])
              + subMatrix[0][2] * (subMatrix[1][0] * subMatrix[2][1] - subMatrix[1][1] * subMatrix[2][0]);

    if (det == 0) {
        throw std::runtime_error("Matrix is singular, cannot invert.");
    }

    // Compute the inverse of the 3x3 submatrix
    float invDet = 1.0f / det;
    float invSubMatrix[3][3];
    invSubMatrix[0][0] =  (subMatrix[1][1] * subMatrix[2][2] - subMatrix[1][2] * subMatrix[2][1]) * invDet;
    invSubMatrix[0][1] = -(subMatrix[0][1] * subMatrix[2][2] - subMatrix[0][2] * subMatrix[2][1]) * invDet;
    invSubMatrix[0][2] =  (subMatrix[0][1] * subMatrix[1][2] - subMatrix[0][2] * subMatrix[1][1]) * invDet;
    invSubMatrix[1][0] = -(subMatrix[1][0] * subMatrix[2][2] - subMatrix[1][2] * subMatrix[2][0]) * invDet;
    invSubMatrix[1][1] =  (subMatrix[0][0] * subMatrix[2][2] - subMatrix[0][2] * subMatrix[2][0]) * invDet;
    invSubMatrix[1][2] = -(subMatrix[0][0] * subMatrix[1][2] - subMatrix[0][2] * subMatrix[1][0]) * invDet;
    invSubMatrix[2][0] =  (subMatrix[1][0] * subMatrix[2][1] - subMatrix[1][1] * subMatrix[2][0]) * invDet;
    invSubMatrix[2][1] = -(subMatrix[0][0] * subMatrix[2][1] - subMatrix[0][1] * subMatrix[2][0]) * invDet;
    invSubMatrix[2][2] =  (subMatrix[0][0] * subMatrix[1][1] - subMatrix[0][1] * subMatrix[1][0]) * invDet;

    // Transpose the inverse of the 3x3 matrix and set it in the result
    TransformationMatrix result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.matrix[i][j] = invSubMatrix[j][i]; // Transpose during assignment
        }
    }

    // Set the remaining elements in the 4x4 result matrix to match the original matrix or 0
    for (int i = 0; i < 4; ++i) {
        result.matrix[i][3] = 0.0f;
        result.matrix[3][i] = 0.0f;
    }
    result.matrix[3][3] = 1.0f;

    return result;
}

};
