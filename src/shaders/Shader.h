#include "../models/ImagePane.h"
#include "../models/Object.h"
#include "../models/Camera.h"
#include "../scene/Scene.h"

class Shader
{
private:
    /* data */
public:
    Scene* scene;
    Shader(Scene* scene);
    ~Shader();
    vector<float> diffuseShadingAt(vector<float> locationü, Object* intersectingObject, int diffuseShadingAt);
    vector<float> ambientShadingAt(vector<float> location, Object* intersectingObject, int intersectingObjIndex);
    vector<float> specularShadingAt(Ray cameraRay,vector<float> location, Object* intersectingObject, int intersectingObjIndex);
    std::vector<float> specularReflection(Ray r, Scene* scene, Object* source_obj,  int remaining_hop, int intersect_index);
    std::vector<float> refractionTransmission(Ray r, Scene* scene, Object* target_obj, int remaining_hop, int intersect_index);
    std::vector<float> radianceAt(std::vector<float> location, Object* intersectingObject, int intersectingObjIndex);
    bool lightHits(Ray light_ray, std::vector<float> location, Object* intersectingObject, int intersectingObjIndex, Object** other_objects, int numobj );
};

