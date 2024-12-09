#include "TextureMap.h"
#include <cmath>
#include <cstring>  // For strcmp

TextureMap::TextureMap(){
    
}
TextureMap::~TextureMap(){
    
}
TextureMap::TextureMap(TextureImage* texture_image, bool is_image, DecalMode decal_mode, InterploationType interpolation_type){
    this->tim = texture_image;
    this->is_image = is_image;
    this->decal_mode = decal_mode;
    this->interpolation_type = interpolation_type;

}

DecalMode TextureMap::getDecalMode(const char* mode){
    if(strcmp(mode, "replace_background") == 0)
        return replace_background;
    if(strcmp(mode, "blend_kd") == 0)
        return blend_kd;
    if(strcmp(mode, "replace_ks") == 0)
        return replace_ks;
    if(strcmp(mode, "replace_kd") == 0)
        return replace_kd;
    if(strcmp(mode, "replace_background") == 0)
        return replace_background;
    if(strcmp(mode, "bump_normal") == 0)
        return bump_normal;
    if(strcmp(mode, "replace_all") == 0)
        return replace_all;
    if(strcmp(mode, "replace_normal") == 0)
        return replace_normal;
    
}



Vec3 TextureMap::interpolateAt(uv loc, InterploationType interploation_type){
    // TODO: Below assumes this is image, is that correct ?_
    return this->getTim()->get_value(loc.u, loc.v, interploation_type);
}

TextureImage* TextureMap::getTim(){
    return this->tim;
}
InterploationType TextureMap::getInterpolationType(const char* mode){
    if(strcmp(mode, "bilinear") == 0)
        return BILINEAR;
    if(strcmp(mode, "nearest") == 0)
        return NEAREAST_NEIGHBOR;
}