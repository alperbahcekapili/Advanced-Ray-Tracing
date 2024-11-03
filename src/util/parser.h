#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include <string>
#include <vector>
#include "../scene/Scene.h"


std::vector<Scene*>loadFromXml(const std::string &filepath);
#endif
