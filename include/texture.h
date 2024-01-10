#pragma once

#include "citro3d.h"
#include <string>
#include "console.h"

class Texture {
    public:
    std::string name;
    C3D_Tex tex;
    C3D_TexCube cube;
    Texture(std::string name) : name(name) {
        // Console::log("texture created");
        // Console::log(name.c_str());
    }
    ~Texture() {
    }
};