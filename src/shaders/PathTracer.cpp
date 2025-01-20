#include "PathTracer.h"
#include "../util/data_structures.h"
#include "../models/Ray.h"
#include "../models/Object.h"

PathTracer::PathTracer(std::vector<PathTracingTechnique> techniques, int splitting_factor)
{
    this->techniques = techniques;
    this->splitting_factor = splitting_factor;
    this->uniform = (techniques.size() == 1 && techniques.at(0) == UNIFORM);
}
PathTracer::~PathTracer()
{
}


Ray* PathTracer::generateInitialRays(Ray* cameraRay, Ray* rays, Object* hit_object, PathTracingTechnique tech){
     
    tech =  directionTechnique();
    for (int i = 0; i < this->splitting_factor; i++)
    {
        // generate random direction
        rays[i] = *getNextRay(*cameraRay, hit_object, hit_object->Intersects(*cameraRay), tech);
    }
    return rays;
}

bool PathTracer::hasTechnique(PathTracingTechnique tech){
    for (int i = 0; i < this->techniques.size(); i++)
    {
        if(techniques.at(i) == tech)
            return true;
    }
    return false;
}


PathTracingTechnique PathTracer::directionTechnique(){
    if(this->uniform)   
        return UNIFORM;
    for (int i = 0; i < this->techniques.size(); i++)
    {
        if(techniques.at(i) == IMPORTANCE_SAMPLING)
            return IMPORTANCE_SAMPLING;
    }
    // Direction is uniform by default
    return UNIFORM;
}



Ray* PathTracer::getNextRay(Ray ray, Object* hit_object, Vec3 hit_location, PathTracingTechnique technique) {
    
    if(technique == UNIFORM)
    {
        // Generate two random numbers for uniform sampling on a hemisphere
        float xi1 = generate_random_01(); // Random value in [0, 1)
        float xi2 = generate_random_01(); // Random value in [0, 1)

        // Convert random values to spherical coordinates
        float theta = acos(xi1);                // Polar angle
        float phi = 2.0 * M_PI * xi2;           // Azimuthal angle

        // Calculate sine and cosine of theta
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        // Get the surface normal at the hit location
        Vec3 n = hit_object->getSurfaceNormal(ray);

        // Calculate tangent and bitangent (local coordinate frame)
        Vec3** tangent_vectors = Ray(Vec3(), n).getTangentVectors();
        Vec3 u = *tangent_vectors[0];  // Tangent
        Vec3 v = *tangent_vectors[1];  // Bitangent

        // Compute the random direction in world space
        Vec3 random_direction = 
            u * sinTheta * cos(phi) +  // x component
            v * sinTheta * sin(phi) +  // y component
            n * cosTheta;              // z component (aligned with the normal)

        // Normalize the random direction
        random_direction = random_direction.normalize();

        // Offset the origin slightly to avoid self-intersections
        Vec3 ray_origin = hit_location + (n * 0.001f);

        // Return the next ray
        return new Ray(ray_origin, random_direction);
    }
    else if (technique == IMPORTANCE_SAMPLING)
    {
                // Generate two random numbers for uniform sampling on a hemisphere
        float xi1 = generate_random_01(); // Random value in [0, 1)
        float xi2 = generate_random_01(); // Random value in [0, 1)

        // Convert random values to spherical coordinates
        float theta = asin(sqrt(xi1));                // Polar angle
        float phi = 2.0 * M_PI * xi2;           // Azimuthal angle

        // Calculate sine and cosine of theta
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        // Get the surface normal at the hit location
        Vec3 n = hit_object->getSurfaceNormal(ray);

        // Calculate tangent and bitangent (local coordinate frame)
        Vec3** tangent_vectors = Ray(Vec3(), n).getTangentVectors();
        Vec3 u = *tangent_vectors[0];  // Tangent
        Vec3 v = *tangent_vectors[1];  // Bitangent

        // Compute the random direction in world space
        Vec3 random_direction = 
            u * sinTheta * cos(phi) +  // x component
            v * sinTheta * sin(phi) +  // y component
            n * cosTheta;              // z component (aligned with the normal)

        // Normalize the random direction
        random_direction = random_direction.normalize();

        // Offset the origin slightly to avoid self-intersections
        Vec3 ray_origin = hit_location + (n * 0.001f);

        // Return the next ray
        return new Ray(ray_origin, random_direction);
    }
    
}