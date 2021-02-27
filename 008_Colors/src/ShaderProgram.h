#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/glm.hpp"
#include "glad/glad.h"


class ShaderProgram{
private:
    unsigned int m_ID{0};
    // Check to see that the shaders are being linked correctly. Prints error if unsuccessful
    bool checkLinkStatus(const unsigned int ID, const std::string errorType) const;
    const std::string readShaderFile(const std::string shaderFilePath) const;
public:
    // the program ID
    unsigned int getID()const { return m_ID; } ;
    operator int() const { return m_ID; };
    //constructor that will read and build the shade
    ShaderProgram(const std::string vertexPath, const std::string fragmentPath);
    ~ShaderProgram();
    // use/activate the shader
    void use ()const;
    // utility uniform functions
    void setBool(const std::string &name, const bool value) const;
    void setInt(const std::string &name, const int value) const;
    void setFloat(const std::string &name, const float value) const;
    void setVec3(const std::string &name, const glm::vec3 &v) const;
    void setMatrix(const std::string &name, const glm::mat4 &xform)const;
};

