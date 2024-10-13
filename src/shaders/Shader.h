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
    float diffuseShadingAt(vector<float> locationü, Object* intersectingObject);
};

