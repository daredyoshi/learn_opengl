#pragma once

#include "glm/glm.hpp"
#include "ShaderProgram.h"



class Material{
private:
    const glm::vec3 m_ambient;
    const glm::vec3 m_diffuse;
    const glm::vec3 m_specular;
    const float m_shininess;
public:
    Material(
            const glm::vec3 ambient,
            const glm::vec3 diffuse,
            const glm::vec3 specular,
            const float shininess
            ) : 
        m_ambient{ambient},
        m_diffuse{diffuse},
        m_specular{specular},
        m_shininess{shininess} {}

    const glm::vec3 getAmbient() const { return m_ambient; }
    const glm::vec3 getDiffuse() const { return m_diffuse; }
    const glm::vec3 getSpecular() const { return m_specular; }
    float getShininess() const { return m_shininess; }

    // this assumes the shader has a matching material struct
    void setShaderMaterial(const ShaderProgram& shader){
        shader.setVec3("material.ambient", m_ambient);
        shader.setVec3("material.diffuse", m_diffuse);
        shader.setVec3("material.specular", m_specular);
        shader.setFloat("material.shininess", m_shininess);
    }
};


class EmeraldMaterial : public Material{
public:
    EmeraldMaterial() : Material(
            glm::vec3{0.0215, 0.1745, 0.0215},
            glm::vec3{0.07568, 0.61424, 0.07568},
            glm::vec3{0.633, 0.727811, 0.633},
            0.6f
            ) {}
};


class JadeMaterial : public Material{
public:
    JadeMaterial() : Material(
            glm::vec3{0.135, 0.2225, 0.1575},
            glm::vec3{0.54, 0.89, 0.63},
            glm::vec3{0.316228, 0.316228, 0.316228},
            0.1f
            ) {}
};

class ObsidianMaterial : public Material{
public:
    ObsidianMaterial() : Material(
            glm::vec3{0.5375, 0.05, 0.06625},
            glm::vec3{0.18275, 0.17, 0.22525},
            glm::vec3{0.332741, 0.328634, 0.346435},
            0.3f
            ) {}
};


