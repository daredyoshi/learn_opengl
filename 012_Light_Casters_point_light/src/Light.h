#pragma once



#include "glm/glm.hpp"
#include "ShaderProgram.h"



class Light{
private:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
public:
    Light(
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular
            ) : 
        m_ambient{ambient},
        m_diffuse{diffuse},
        m_specular{specular}
        {}

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
        Light(ambient, diffuse, specular),
        m_direction{direction}
    {
        // make sure the vector is normalized
        assert((glm::length(direction) == 1.0));
    }
         
    const glm::vec3 getDirection() const { return m_direction; }
    void setDirection(const glm::vec3 direction) { m_direction = direction; }
    // direction lights have no position

    virtual void setShaderMaterial(const ShaderProgram& shader) const override {
        Light::setShaderMaterial(shader);
        shader.setVec3("light.direction", m_direction);
    }
};


class PointLight : public Light{
private:
    glm::vec3 m_position;
    // generally I don't think these would animate?
    const float m_linear;
    const float m_quadratic;

public:
    PointLight(
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            glm::vec3 position,

            float linear,
            float quadratic
            ) :
        Light(ambient, diffuse, specular),
        m_position{position},
        m_linear{linear},
        m_quadratic{quadratic} {}
    
    const glm::vec3 getPosition() const { return m_position; }
    void setPosition(const glm::vec3 position) { m_position = position; }

    virtual void setShaderMaterial(const ShaderProgram& shader) const override {
        Light::setShaderMaterial(shader);
        shader.setVec3("light.position", m_position);
        shader.setFloat("light.linear", m_linear);
        shader.setFloat("light.quadratic", m_quadratic);
    }
};


