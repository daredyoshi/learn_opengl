#pragma once



#include "glm/glm.hpp"
#include "ShaderProgram.h"



class Light{
private:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    glm::vec3 m_position;
public:
    Light(
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            glm::vec3 position
            ) : 
        m_ambient{ambient},
        m_diffuse{diffuse},
        m_specular{specular},
        m_position{position} {}

    const glm::vec3 getAmbient() const { return m_ambient; }
    const glm::vec3 getDiffuse() const { return m_diffuse; }
    const glm::vec3 getSpecular() const { return m_specular; }
    const glm::vec3 getPosition() const { return m_position; }

    void setPosition(const glm::vec3 position) { m_position = position; }
    void setAmbient(const glm::vec3 ambient) { m_ambient= ambient; }
    void setDiffuse(const glm::vec3 diffuse) { m_diffuse = diffuse; }
    void setSpecular(const glm::vec3 specular) { m_specular= specular; }

    // this assumes the shader has a matching light struct
    void setShaderMaterial(const ShaderProgram& shader){
        shader.setVec3("light.ambient", m_ambient);
        shader.setVec3("light.diffuse", m_diffuse);
        shader.setVec3("light.specular", m_specular);
        shader.setVec3("light.position", m_position);
    }
};
