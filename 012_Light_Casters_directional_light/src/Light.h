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

    void setAmbient(const glm::vec3 ambient) { m_ambient= ambient; }
    void setDiffuse(const glm::vec3 diffuse) { m_diffuse = diffuse; }
    void setSpecular(const glm::vec3 specular) { m_specular= specular; }

    // this assumes the shader has a matching light struct
    virtual void setShaderMaterial(const ShaderProgram& shader) const{
        shader.setVec3("light.ambient", m_ambient);
        shader.setVec3("light.diffuse", m_diffuse);
        shader.setVec3("light.specular", m_specular);
        shader.setVec3("light.position", m_position);
    }
};


class DirectionalLight : public Light{
private:
    glm::vec3 m_direction;

public:
    DirectionalLight(
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            glm::vec3 direction 
            ) : 
        Light(ambient, diffuse, specular, glm::vec3{0,0,0}),
        m_direction{direction}
    {
        // make sure the vector is normalized
        assert((glm::length(direction) == 1.0));
    }
         
    const glm::vec3 getDirection() const { return m_direction; }
    void setDirection(glm::vec3 direction) { m_direction = direction; }
    // direction lights have no position

    virtual void setShaderMaterial(const ShaderProgram& shader) const override {
        // it doesn't matter that it also sets the position 
        // attribute in the struct. 
        Light::setShaderMaterial(shader);
        shader.setVec3("light.direction", m_direction);
    }
};
