#include "data_structures.h"
#include <ctime>  
#include <chrono>
float generate_random_01() {
    // Static variables persist across function calls
    static std::mt19937 gRandomGenerator(
        std::chrono::system_clock::now().time_since_epoch().count());
    static std::uniform_real_distribution<float> gNURandomDistribution(0.0f, 1.0f);
    
    return gNURandomDistribution(gRandomGenerator);
}