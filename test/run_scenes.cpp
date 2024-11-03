#include <iostream>
#include <vector>
#include <string>
#include <cstring>  // For C-style string functions

#include <chrono>
#include <cstdlib>


std::string getFilenameWithoutExtension(const std::string& path) {
    // Find the last occurrence of '/' to get the start of the filename
    size_t lastSlash = path.find_last_of('/');
    size_t start = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;

    // Find the last occurrence of '.' to remove the extension
    size_t lastDot = path.find_last_of('.');
    size_t end = (lastDot == std::string::npos) ? path.length() : lastDot;

    return path.substr(start, end - start);
}



std::vector<float> run_all(std::string executable){
    std::vector<float> execTimes;
     std::vector<std::string> scenes = {
        "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/bunny.xml",
        "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/chinese_dragon.xml",
        "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/cornellbox_recursive.xml",
        "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/cornellbox.xml",
        "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/other_dragon.xml",
        "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/scienceTree_glass.xml",
        "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/scienceTree.xml",
        "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/simple.xml",
        "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/spheres_mirror.xml",
        "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/spheres.xml",
        "/home/alpfischer/Advanced-Ray-Tracing/src/hw1/inputs/two_spheres.xml"
    };


    for (size_t i = 0; i < scenes.size(); i++)
    {   


        std::cout << "Starting following scene: " << scenes.at(i) << " \n";
        // Start timing
        auto start = std::chrono::steady_clock::now();
        
        int result = std::system((executable + " " + scenes.at(i)).c_str());    
          // End timing
        auto end = std::chrono::steady_clock::now();
        
        // Calculate duration in seconds
        std::chrono::duration<float> duration = end - start;
        execTimes.push_back(duration.count()); // Store the execution time in seconds
  
    }
    
    // Print execution times
    std::cout << "\n\n Execution times (in seconds): \n";
    for (size_t i = 0; i < scenes.size(); i++)
    {
        
        std::cout << getFilenameWithoutExtension(scenes.at(i)) <<": " <<  execTimes.at(i) << " \n";
    }
    
    for (float time : execTimes) {
        
    }


    return execTimes;


}

int main(int argc, char const *argv[])
{
    run_all(argv[1]);
    return 0;
}
