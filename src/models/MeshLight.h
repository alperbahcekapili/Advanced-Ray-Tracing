#include "../util/data_structures.h"
#include "Mesh.h"
#include "../lights/Light.h"

class MeshLight: public Mesh, public Light
{
private:
    /* data */
public:
    float area;
    MeshLight(Material* material, ObjectType objectType, Vec3* faces, int numfaces, TransformationMatrix* tm, int num_tex_maps, TextureMap* texture_maps, std::vector<std::pair<float, float> > uv_coords_mesh, Vec3 radiance);
    ~MeshLight();
    Vec3 irradianceAt(Ray light_ray, Vec3  location) ;
    Vec3 getPointOn();
    Vec3 getBoundingBox(bool isMax) override;

};

