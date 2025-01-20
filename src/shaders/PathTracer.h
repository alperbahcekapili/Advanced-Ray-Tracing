#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "stdio.h"
#include "../models/Ray.h"
#include "../models/Object.h"
#include "../acceleration/BVH.h"




using namespace std;

enum PathTracingTechnique
{
    UNIFORM,
    NEE,
    RUSSIAN_ROULETTE,
    IMPORTANCE_SAMPLING,
};


// forward declarations to prevent circular dependencies


class PathTracer
{
private:
    /* data */
public:
    PathTracer(std::vector<PathTracingTechnique> techniques, int splitting_factor);
    ~PathTracer();
    std::vector<PathTracingTechnique> techniques;
    int splitting_factor;
    bool uniform;
    Ray* generateInitialRays(Ray* cameraRay, Ray* rays, Object* hit_object, PathTracingTechnique tech); // creates splitting factor many initial rays acc. to techniques
    Ray* getNextRay(Ray ray, Object* hit_object, Vec3 hit_location, PathTracingTechnique technique);
    PathTracingTechnique directionTechnique();
    bool hasTechnique(PathTracingTechnique tech);

    
};
