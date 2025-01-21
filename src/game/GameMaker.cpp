#include "GameMaker.h"


int GameMaker::render_scene(){
    
    Scene* curscene = this->scene;
    BVH* bvh = new BVH(curscene->sceneObjects, curscene->numObjects, 0);
    for (int j = 0; j < curscene->numObjects; j++)
    {
        curscene->sceneObjects[j]->id = j;
    }
    bvh->visualize(0);
    curscene->bvh = bvh;
    Shader shader = Shader(
        curscene
    );
    int imgWidth= curscene->imagePane->dimx;
    int imgHeight = curscene->imagePane->dimy;
    Vec3** image = new Vec3*[imgWidth];
    int total_progress = 0;
    // #pragma omp parallel for
    for (int i = 0; i < imgWidth; i++)
    {
        image[i] = new Vec3[imgHeight];
        // printf("Totatl progress: %f\n", float(total_progress)/(imgHeight*imgWidth));
        for (int j = 0; j < imgHeight; j++)
        {
            total_progress++;
            Vec3 cumulative_pixel = Vec3(0,0,0);
            for (int rayindex = 0; rayindex < curscene->camera->numsamples; rayindex++)
            {
            
            if(i == 150 && j == 180)
                printf("Alper");
            Ray cameraRay = curscene->imagePane->rayFromCamera(i, j, rayindex);
            float minTValue = 9999999;
            float maxTValue = -1;
            Object* tofill = nullptr;
            bool intersected = bvh->intersectObject(cameraRay, tofill, minTValue, maxTValue);
            int intersectingObjIndex = -1;
            if(intersected)
                intersectingObjIndex = tofill -> id;                    
            if (intersectingObjIndex == -1){
                // if scene has bg texture then use it
                Vec3 bg_pixel_val = Vec3(float(shader.scene->bg.x),float(shader.scene->bg.y),float(shader.scene->bg.z));
                if(shader.scene->bg_texture_set){
                    bool topleft = false;
                    if(i < imgWidth/2  && j < imgHeight/2)
                        topleft = true;
                    // intersection point is defined by pixel indices
                    Vec3 intersection_point = Vec3(i, j, 1);
                    Vec3 v1 = topleft ? Vec3(0,0,0) : Vec3(imgWidth, imgHeight, 1);
                    Vec3 v2 = Vec3(0, imgHeight, 1);
                    Vec3 v3 = Vec3(imgWidth, 0, 1);
                    std::pair<double, double> uv_a = {0, 0}, uv_b = {1, 0}, uv_c = {0, 1};
                    if(! topleft)
                        uv_a = {1,1};
                    uv tmp_uv = uv::calculateUVTriangle(intersection_point, v1, v2, v3, uv_a, uv_b, uv_c);
                    bg_pixel_val = shader.scene->bg_texture->interpolateAt(tmp_uv, NEAREAST_NEIGHBOR) * 255 ;   
                }
                cumulative_pixel = cumulative_pixel +  (bg_pixel_val/curscene->camera->numsamples);
                image[i][j] = clipValues(cumulative_pixel, 255.0);
                continue;
            }
            Vec3  diffuse_intensity = shader.diffuseShadingAt(cameraRay.locationAtT(minTValue), tofill, intersectingObjIndex);
            Vec3  ambient_intensity = shader.ambientShadingAt(cameraRay.locationAtT(minTValue), tofill, intersectingObjIndex);
            Vec3  specular_intensity = shader.specularShadingAt(cameraRay, cameraRay.locationAtT(minTValue) , tofill, intersectingObjIndex);

            Vec3  pixel_val = specular_intensity + diffuse_intensity + ambient_intensity;
            cumulative_pixel = cumulative_pixel + (pixel_val/curscene->camera->numsamples);
            }
            image[i][j] = cumulative_pixel;
        }
    }
    float* flatArray = new float[imgWidth * imgHeight * 3];
    convertVec3ToFlatArray(image, imgWidth, imgHeight, flatArray);
    unsigned width = imgWidth, height = imgHeight;
    std::vector<unsigned char> image_mat(width * height * 3, 255);
    for (int x = 0; x < imgWidth * imgHeight * 3; ++x) {
        image_mat[x] = static_cast<uint8_t>(flatArray[x]);
    }
    size_t pos = curscene->camera->name.find(".exr");
    if (pos != std::string::npos) {
        curscene->camera->name.replace(pos, 4, ".png");
    }
    unsigned error = lodepng::encode(curscene->camera->name, image_mat, width, height, LCT_RGB);
    if (error) {
        std::cerr << "Error encoding PNG: " << lodepng_error_text(error) << std::endl;
        return 0;
    } else {
        std::cout << "Scene is created \n";
        return 1;
    }
}



int GameMaker::placeObjectTo(Object* object, pair<int, int> tile, int num_objects){
    Vec3 cumulative_center = Vec3(0,0,0);
    for (size_t i = 0; i < num_objects; i++)
    {
        cumulative_center = cumulative_center + object[i].getCenter();
    }
    cumulative_center = cumulative_center / num_objects;
    Vec3 object_pos = cumulative_center;
    Vec3 tile_pos;
    tile_pos.x = this->tile_centers[tile.first*4 + tile.second][0];
    tile_pos.y = this->tile_centers[tile.first*4 + tile.second][1];
    tile_pos.z = this->tile_centers[tile.first*4 + tile.second][2];

    Vec3 translation = tile_pos - object_pos; // TODO add y offset to get above the board
    TransformationMatrix tm = TransformationMatrix(translation, 't');
    for (size_t i = 0; i < num_objects; i++)
    {
        *(object[i].gettm()) = *(object[i].gettm()) * tm;
    }
    return 1; // TODO: we udpate object's tm here locally. However, we need 
}


int GameMaker::wait_for_messages(){
    // Read the response from the Python program
    std::ifstream read_pipe(pipe_out);
    if (!read_pipe) {
        std::cerr << "Failed to open read pipe.\n";
        return 1;
    }
    std::string response;
    std::getline(read_pipe, response);
    std::cout << "C++ received: " << response << std::endl;
    read_pipe.close();
    
    

    // Write to the Python program
    std::ofstream write_pipe(pipe_in);
    if (!write_pipe) {
        std::cerr << "Failed to open write pipe.\n";
        return 1;
    }
    std::string message = "Hello from C++!";
    std::cout << "C++ sending: " << message << std::endl;
    write_pipe << message << std::endl;
    write_pipe.close();
    return 0;


}


GameMaker::GameMaker(Scene* scene)
{

    this->scene = scene;
    std::cout << "I set my scene: \n";
    
    /*
    Reads xml file and create object instances out of the base objects
    */

    // walls
    int wall_material = 3;
    Object* base_wall_1 = scene->sceneObjects[32];
    ObjectInstance* new_obj  = new ObjectInstance(base_wall_1, false, new TransformationMatrix(), scene->materials.at(wall_material));
    this->placeObjectTo(new_obj, pair<int, int>(1,3), 1); // This should move the new object to 0,0 tile 
    scene->sceneObjects[0] = new_obj;
    
    
    this->render_scene();
    

    

}

GameMaker::~GameMaker()
{
}
