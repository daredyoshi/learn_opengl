#pragma once

#include "glm/glm.hpp"
#include "ShaderProgram.h"
#include "Texture.h"


class Material{
private:
    const std::string m_name;
    const glm::vec3 m_ambient;
    const glm::vec3 m_diffuse;
    const glm::vec3 m_specular;
    const float m_shininess;
    Texture m_diffuseTex1 {};
    Texture m_diffuseTex2 {};
    Texture m_diffuseTex3 {};
    Texture m_specularTex {};

public:
    Material(
            const std::string name,
            const glm::vec3 ambient,
            const glm::vec3 diffuse,
            const glm::vec3 specular,
            const float shininess
            ) : 
        m_name{name},
        m_ambient{ambient},
        m_diffuse{diffuse},
        m_specular{specular},
        m_shininess{shininess}
        {}

    const glm::vec3 getAmbient() const { return m_ambient; }
    const glm::vec3 getDiffuse() const { return m_diffuse; }
    const glm::vec3 getSpecular() const { return m_specular; }
    float getShininess() const { return m_shininess; }
    const Texture getDiffuseTex1() const { return m_diffuseTex1; }
    const Texture getDiffuseTex2() const { return m_diffuseTex2; }
    const Texture getSpecularTex() const { return m_specularTex; }

    void setDiffuseTex1(const Texture tex){ m_diffuseTex1 = tex; }
    void setDiffuseTex2(const Texture tex){ m_diffuseTex2 = tex; }
    void setDiffuseTex3(const Texture tex){ m_diffuseTex2 = tex; }
    void setSpecularTex(const Texture tex){ m_specularTex = tex; }

    // this assumes the shader has a matching material struct
    void setShaderMaterial(const ShaderProgram& shader){


        // FOR SOME REASON NON OF THESE WORK BUT SEGFAULT
        //
        // Running through the debugger it's not the first call
        // but the second loop where it segfaults
        //
        // shader.setVec3(m_name + ".ambient", m_ambient);
        // shader.setVec3(m_name + ".diffuse", m_diffuse);
        // shader.setVec3(m_name + ".specular", m_specular);
        // shader.setFloat(m_name + ".shininess", m_shininess);
        // shader.setInt(m_name + ".diffuseTex1", 0);
        // shader.setInt(m_name + ".diffuseTex2", 1);
        // shader.setInt(m_name + ".diffuseTex3", 2);
        // shader.setInt(m_name + ".specularTex", 3);
        //
        // The following does not segfault. 
        //
        shader.setVec3("material.ambient", m_ambient);

    }
    void setShaderTextures(){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_diffuseTex1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_diffuseTex2); 
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_diffuseTex3); 
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_specularTex);
    }
};


class EmeraldMaterial : public Material{
public:
    EmeraldMaterial(const std::string name) : Material(
            name,
            glm::vec3{0.0215, 0.1745, 0.0215},
            glm::vec3{0.07568, 0.61424, 0.07568},
            glm::vec3{0.633, 0.727811, 0.633},
            20.0f
            ) {}
};


class JadeMaterial : public Material{
public:
    JadeMaterial(const std::string name) : Material(
            name,
            glm::vec3{0.135, 0.2225, 0.1575},
            glm::vec3{0.54, 0.89, 0.63},
            glm::vec3{0.316228, 0.316228, 0.316228},
            32.0f
            ) {}
};

class ObsidianMaterial : public Material{
public:
    ObsidianMaterial(const std::string name) : Material(
            name,
            glm::vec3{0.5375, 0.05, 0.06625},
            glm::vec3{0.18275, 0.17, 0.22525},
            glm::vec3{0.332741, 0.328634, 0.346435},
            32.0f
            ) {}
};


