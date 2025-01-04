#include "../models/ImagePane.h"
#include "../models/Object.h"
#include "../models/Camera.h"
#include "../scene/Scene.h"
#include "../util/data_structures.h"

class Shader
{
private:
    /* data */
public:
    Scene* scene;
    Shader(Scene* scene);
    ~Shader();
    
    Vec3  diffuseShadingAt(Vec3  location, Object* intersectingObject, int diffuseShadingAt);
    Vec3  ambientShadingAt(Vec3  location, Object* intersectingObject, int intersectingObjIndex);
    Vec3  specularShadingAt(Ray cameraRay,Vec3  location, Object* intersectingObject, int intersectingObjIndex);
    Vec3  specularReflection(Ray r, Scene* scene, Object* source_obj,  int remaining_hop, int intersect_index);
    Vec3  refractionTransmission(Ray r, Scene* scene, Object* target_obj, int remaining_hop, int intersect_index);
    Vec3 BRDFShadingAt(Vec3  location, Object* intersectingObject, int intersectingObjIndex);
    Vec3  radianceAt(Vec3  location, Object* intersectingObject, int intersectingObjIndex);
    bool lightHits(Ray light_ray, Vec3  location, Object* intersectingObject, int intersectingObjIndex, Object** other_objects, int numobj );
};

