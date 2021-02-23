#pragma once

#include <string>
#include "glad/glad.h"

class Texture{
private:
    unsigned int m_ID{0};

public:
    unsigned int getID() const{ return m_ID; };
    operator int() const { return m_ID; };
    Texture(const std::string texturePath);
};



