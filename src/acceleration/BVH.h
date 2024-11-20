#pragma once

#include "../models/Object.h"
#include "../util/data_structures.h"

class BVH
{
private:
    void quickSort(Object** objects, int low, int high, int axis);
    int partition(Object** objects, int low, int high, int axis);

public:
    BVH* left;
    BVH* right;
    Vec3 min;
    Vec3 max;
    bool is_leaf;
    Object* leaf_object;
    int total_depth;


    Vec3 calculateBounds(Object** objects, int num_objects, bool isMax) {
        Vec3 bounds = objects[0]->getBoundingBox(isMax);
        for (int i = 1; i < num_objects; ++i) {
            Vec3 currentBound = objects[i]->getBoundingBox(isMax);
            bounds.x = isMax ? std::max(bounds.x, currentBound.x) : std::min(bounds.x, currentBound.x);
            bounds.y = isMax ? std::max(bounds.y, currentBound.y) : std::min(bounds.y, currentBound.y);
            bounds.z = isMax ? std::max(bounds.z, currentBound.z) : std::min(bounds.z, currentBound.z);
        }
        return bounds;
    }

    BVH(Object** objects, int num_objects, int depth);
    bool intersectObject(Ray ray, Object*& to_fill, float& tmin, float& tmax);
    bool intersects(Ray r, float& tNear, float& tFar);
    ~BVH();
    void visualize(int level) const;
};
