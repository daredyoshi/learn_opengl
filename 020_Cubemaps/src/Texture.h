#pragma once

#include <string>
#include "glad/glad.h"

class Texture{
private:
    std::string m_path{};
    unsigned int m_ID{0};
    void init();

public:
    unsigned int getID() const{ return m_ID; };
    operator int() const { return m_ID; };

    Texture(const std::string texturePath);
    Texture();

    std::string getPath() const { return m_path; };
    void load(const std::string texturePath);
};




