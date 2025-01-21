#include "src/lights/PointLight.h"
#include "src/models/ImagePane.h"
#include "src/models/Triangle.h"
#include "src/models/Material.h"
#include "src/shaders/Shader.h"
#include "src/models/Camera.h"
#include "src/models/Sphere.h"
#include "src/lights/Light.h"
#include "src/scene/Scene.h"
#include "src/models/Ray.h"
#include "src/util/util.h"
#include "src/util/lodepng.h"
#include "src/util/parser.h"
#include "src/acceleration/BVH.h"
#include "src/game/GameMaker.h"

#include <algorithm>
#include <cmath> // For fmod
#include <stdio.h>


int main(int argc, char const *argv[])
{

    if(argc<1)
    {
        printf("Please give a xml file...");
        return -1;
    }
    std::string fp = std::string(argv[1]);
    std::cout << "File path: " << fp << std::endl;
    std::vector<Scene*> scenes = loadFromXml(fp);
    std::cout << "xml loaded\n";

    GameMaker* gm = new GameMaker(scenes.at(0));
    gm->render_scene();



    
}

