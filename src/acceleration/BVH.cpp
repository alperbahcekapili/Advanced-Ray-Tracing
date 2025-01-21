#include "BVH.h"
#include <iostream>
#include <limits>
#include <utility> // For std::swap
#include <vector>
#include "../util/util.h"

void swap(Object** list, int i, int j) {
    Object* temp = list[i];  // Store the pointer in a temp variable
    list[i] = list[j];           // Assign the value of b to a
    list[j] = temp;        // Assign the temp variable to b
    // std::cout << "In swapping function i:" << i << ", j: " << j << "\n";
}


// QuickSort to sort objects along the given axis
void BVH::quickSort(Object** objects, int low, int high, int axis) {
    if (low < high) {
        int pi = partition(objects, low, high, axis);
        quickSort(objects, low, pi - 1, axis);  // Recursively sort elements before partition
        quickSort(objects, pi + 1, high, axis); // Recursively sort elements after partition
    }
}

// Partition function for QuickSort
int BVH::partition(Object** objects, int low, int high, int axis) {
    Vec3 pivotCenter = objects[high]->getCenter(); // Use the last object as the pivot
    float pivotValue = (axis == 0) ? pivotCenter.x : (axis == 1) ? pivotCenter.y : pivotCenter.z;
    int i = low - 1; // Index of the smaller element

    for (int j = low; j < high; j++) {
        float currentValue = (axis == 0) ? objects[j]->getCenter().x : 
                             (axis == 1) ? objects[j]->getCenter().y : 
                             objects[j]->getCenter().z;
                             
        if (currentValue < pivotValue) {
            i++; // Increment index of smaller element
            //printObjectList(objects, high - low);
            swap(objects, i, j); // Use custom swap
            //printObjectList(objects, high - low);

            
        }
    }
    swap(objects, i + 1, high); // Use custom swap for pivot
    return i + 1; // Return the index of the pivot
}


BVH::BVH(Object** objects, int num_objects, int depth)
{   

    this->is_leaf = false;

     // Calculate bounds for current node
    this->min = calculateBounds(objects, num_objects, false);
    this->max = calculateBounds(objects, num_objects, true);

    // Base case: create a leaf node if there's only one object
    if (num_objects == 1){
        this->is_leaf = true;
        this->leaf_object = objects[0];
        left = nullptr;
        right = nullptr;
        return;
    }

    // Determine the splitting axis (x=0, y=1, z=2) based on depth
    int axis = depth % 3;

    // Sort objects by their center along the splitting axis using QuickSort
    
    quickSort(objects, 0, num_objects - 1, axis);


    // Split objects into two halves
    int mid = num_objects / 2;

    // Recursively build the left and right subtrees
    left = new BVH(objects, mid, depth + 1);
    right = new BVH(objects + mid, num_objects - mid, depth + 1);
    this->total_depth = std::max(left->total_depth, right->total_depth) + 1;
    

}

BVH::~BVH()
{
}

// Visualization function implementation
void BVH::visualize(int level) const {
    // Print the indentation based on the level in the tree
    std::string indent(level * 4, ' '); // Indent by 4 spaces per level

    // Print the bounding box of the current node
    std::cout << indent << "Is leaf: " << this->is_leaf <<", Node Bounding Box: ["
              << "Min: (" << min.x << ", " << min.y << ", " << min.z << "), "
              << "Max: (" << max.x << ", " << max.y << ", " << max.z << ")]"
              << std::endl;

    // Recursively visualize the left and right children if they exist
    if (left != nullptr) {
        std::cout << indent << "Left Child:" << std::endl;
        left->visualize(level + 1); // Recursively visualize left child
    }
    if (right != nullptr) {
        std::cout << indent << "Right Child:" << std::endl;
        right->visualize(level + 1); // Recursively visualize right child
    }
}



bool BVH::intersects(Ray ray, float& tNear, float& tFar){
    // Initialize near and far intersection distances
    tNear = std::numeric_limits<float>::lowest();
    tFar = std::numeric_limits<float>::max();

    // Iterate over each axis (x, y, z)
    for (int axis = 0; axis < 3; ++axis) {
        // Get the ray origin and direction components for the current axis
        float rayOriginAxis = (axis == 0) ? ray.o.x : (axis == 1) ? ray.o.y : ray.o.z;
        float rayDirectionAxis = (axis == 0) ? ray.d.x : (axis == 1) ? ray.d.y : ray.d.z;

        // Get the box min and max values for the current axis
        float boxMinAxis = (axis == 0) ? this->min.x : (axis == 1) ? this->min.y : this->min.z;
        float boxMaxAxis = (axis == 0) ? this->max.x : (axis == 1) ? this->max.y : this->max.z;

        // Calculate the intersection distances for the near and far planes of the box along this axis
        float invRayDir = 1.0f / rayDirectionAxis;
        float t1 = (boxMinAxis - rayOriginAxis) * invRayDir;
        float t2 = (boxMaxAxis - rayOriginAxis) * invRayDir;

        // Swap t1 and t2 if invRayDir is negative
        if (t2 < t1) std::swap(t1, t2);

        // Update tNear and tFar for this axis
        tNear = std::max(tNear, t1);
        tFar = std::min(tFar, t2);

        
    }
    // If at any point tNear exceeds tFar, there is no intersection
    if (tNear > tFar) return false;
    return true;
}


bool BVH::intersectObject(Ray ray, Object*& to_fill, float& tmin, float& tmax){
    if(!this->intersects(ray, tmin, tmax)){
        return false;
    }

    if(this->is_leaf){
        // if current node is leaf then return obj
        // if current bbox intersects with the object then we need to test wheter ray intersects with the object
        float tval = this->leaf_object->Intersects(ray);
        if(tval > 0){
            tmin=tval;
            to_fill = this->leaf_object;
            // std::cout << "Intersection detected at leaf, returning true...\n";
            return true;
        }
        return false;
    }

    // std::cout << "Both of the boxes intersect. tmin for l: " <<  tminl << ", tmin for r: " << tminr << "\n";
    // Then we need to test up until the leave for these two

    Object* to_fill_l = nullptr; 
    Object* to_fill_r = nullptr;
    
    float tminl = std::numeric_limits<float>::lowest();
    float tmaxl = std::numeric_limits<float>::max(); 
    float tminr = std::numeric_limits<float>::lowest();
    float tmaxr = std::numeric_limits<float>::max();

    bool lintersects = this->left->intersectObject(ray, to_fill_l, tminl, tmaxl);
    bool rintersects = this->right->intersectObject(ray, to_fill_r, tminr, tmaxr);
    
    // std::cout << "Tminl: " << tminl << ", tmaxl: " << tmaxl << ", tminr: " << tminr << ", tmaxr: " << tmaxr << "\n";
    if(lintersects && rintersects){
        if(tminl > tminr){
            tmin = tminl;
            tmax = tmaxl;
            to_fill = to_fill_l;
            return true;
        }
        tmin = tminr;
        tmax = tmaxr;
        to_fill = to_fill_r;
        return true;
    }else if (!lintersects && rintersects)
    {
        tmin = tminr;
        tmax = tmaxr;
        to_fill = to_fill_r;
        return true;
    }else if (lintersects && !rintersects)
    {
        tmin = tminl;
        tmax = tmaxl;
        to_fill = to_fill_l;
        return true;
    }
    return false;
    
    
    
    

}
