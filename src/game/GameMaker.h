#pragma once
#include "../lights/PointLight.h"
#include "../models/ImagePane.h"
#include "../models/Triangle.h"
#include "../models/Material.h"
#include "../shaders/Shader.h"
#include "../models/Camera.h"
#include "../models/Sphere.h"
#include "../lights/Light.h"
#include "../scene/Scene.h"
#include "../models/Ray.h"
#include "../util/util.h"
#include "../util/lodepng.h"
#include "../util/parser.h"
#include "../acceleration/BVH.h"
#include "../models/ObjectInstance.h"

#include <algorithm>
#include <cmath> // For fmod
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

enum tile_animation{
    FIRE=1,
    HOVER=2,
    SELECT=3,
    ATTACKER=4,
    UNDER_ATTACK=5
};






struct player
{
    // 2 object for each of the pieces
    Object** berserkers = new Object*[2];
    Object** dragons = new Object*[2];
    Object** walls = new Object*[2];
};

struct board
{
    // There are 32 tiles at total
    Object** tiles = new Object*[8*4];
};



class GameMaker
{
private:
    /* data */
public:

    const std::string pipe_in = "/tmp/pipe_in";
    const std::string pipe_out = "/tmp/pipe_out";

    pair<int,int> cursor;
    



    GameMaker(Scene* scene);
    ~GameMaker();

    int render_scene();
    int placeObjectTo(Object* object, pair<int, int> tile, int num_objects);
    int wait_for_messages();
    int move_cursor(string movement);

    player player1;
    player player2;
    board game_board;

    Scene* scene;

    // defines default material id's for setting instances materials
    int berserker_materials[18] = {3, 9, 10, 11, 4, 5, 4, 6, 5, 6, 7, 6, 7, 8, 1, 2, 6, 5};
    int dragon_material[1] = {14};
    

    int num_material_berserker = 18;
    int num_material_dragon = 1;
    


    float tile_centers[32][3] = {{-87.5,25.0,-87.5},
    {-62.5,25.0,-87.5},
    {-37.5,25.0,-87.5},
    {-12.5,25.0,-87.5},
    {-87.5,25.0,-62.5},
    {-62.5,25.0,-62.5},
    {-37.5,25.0,-62.5},
    {-12.5,25.0,-62.5},
    {-87.5,25.0,-37.5},
    {-62.5,25.0,-37.5},
    {-37.5,25.0,-37.5},
    {-12.5,25.0,-37.5},
    {-87.5,25.0,-12.5},
    {-62.5,25.0,-12.5},
    {-37.5,25.0,-12.5},
    {-12.5,25.0,-12.5},
    {-87.5,25.0,12.5},
    {-62.5,25.0,12.5},
    {-37.5,25.0,12.5},
    {-12.5,25.0,12.5},
    {-87.5,25.0,37.5},
    {-62.5,25.0,37.5},
    {-37.5,25.0,37.5},
    {-12.5,25.0,37.5},
    {-87.5,25.0,62.5},
    {-62.5,25.0,62.5},
    {-37.5,25.0,62.5},
    {-12.5,25.0,62.5},
    {-87.5,25.0,87.5},
    {-62.5,25.0,87.5},
    {-37.5,25.0,87.5},
    {-12.5,25.0,87.5}};




};

