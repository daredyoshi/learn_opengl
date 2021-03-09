#pragma once


#include <string>
#include <string_view>
#include "glm/glm.hpp"
#include "ShaderProgram.h"



class Light{
private:
    const std::string m_name;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
public:
    Light(
            const std::string name,
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular
            ) : 
        m_name{name},
        m_ambient{ambient},
        m_diffuse{diffuse},
        m_specular{specular}
        {}

    const std::string getName() const { return m_name; }
    const glm::vec3 getAmbient() const { return m_ambient; }
    const glm::vec3 getDiffuse() const { return m_diffuse; }
    const glm::vec3 getSpecular() const { return m_specular; }

    void setAmbient(const glm::vec3 ambient) { m_ambient= ambient; }
    void setDiffuse(const glm::vec3 diffuse) { m_diffuse = diffuse; }
    void setSpecular(const glm::vec3 specular) { m_specular= specular; }

    // this assumes the shader has a matching light struct
    virtual void setShaderMaterial(const ShaderProgram& shader) const{
        shader.setVec3(m_name + ".ambient", m_ambient);
        shader.setVec3(m_name + ".diffuse", m_diffuse);
        shader.setVec3(m_name + ".specular", m_specular);
    }
};


class DirectionalLight : public Light{
private:
    glm::vec3 m_direction;

public:
    DirectionalLight(
            const std::string name,
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            glm::vec3 direction 
            ) : 
        Light(name, ambient, diffuse, specular),
        m_direction{direction}
    {
        // make sure the vector is normalized
        // assert((glm::length(direction) == 1.0));
    }
         
    const glm::vec3 getDirection() const { return m_direction; }
    void setDirection(const glm::vec3 direction) { m_direction = direction; }
    // direction lights have no position

    virtual void setShaderMaterial(const ShaderProgram& shader) const override {
        Light::setShaderMaterial(shader);
        const std::string name { Light::getName() };
        shader.setVec3(name + ".direction", m_direction);
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
            const std::string name,
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            glm::vec3 position,

            float linear,
            float quadratic
            ) :
        Light(name, ambient, diffuse, specular),
        m_position{position},
        m_linear{linear},
        m_quadratic{quadratic} {}
    
    const glm::vec3 getPosition() const { return m_position; }
    void setPosition(const glm::vec3 position) { m_position = position; }

    virtual void setShaderMaterial(const ShaderProgram& shader) const override {
        Light::setShaderMaterial(shader);
        const std::string name { Light::getName() };
        shader.setVec3(name + ".position", m_position);
        shader.setFloat(name + ".linear", m_linear);
        shader.setFloat(name + ".quadratic", m_quadratic);
    }
};

class SpotLight: public PointLight {
private:
    glm::vec3 m_direction;
    const float m_cutoff;
    const float m_outerCutoff;

public:
    SpotLight(
            const std::string name, 
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            glm::vec3 position,
            glm::vec3 direction,

            float linear,
            float quadratic,
            float cutoff,
            float outerCutoff 
            ) :
        PointLight(name, ambient, diffuse, specular, position, linear, quadratic),
        m_direction{direction},
        m_cutoff{cutoff},
        m_outerCutoff{outerCutoff} {}

    const glm::vec3 getDirection() const { return m_direction; }
    void setDirection(const glm::vec3 direction) { m_direction = direction; }

    float getCutoff() const { return m_cutoff; }

    virtual void setShaderMaterial(const ShaderProgram& shader) const override {
        PointLight::setShaderMaterial(shader);
        const std::string name { Light::getName() };
        shader.setVec3(name + ".direction", m_direction);
        // calculate the cosine value based on an angle,
        // pass that into fragment shader (saves a few calculations)
        shader.setFloat(name + ".cutoff", glm::cos(glm::radians(m_cutoff)));
        shader.setFloat(name + ".outerCutoff", glm::cos(glm::radians(m_outerCutoff)));
    }


};

