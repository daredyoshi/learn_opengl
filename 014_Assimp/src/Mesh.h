#pragma once

#include <vector>


#include "Materials.h"
#include "glm/glm.hpp"
#include "ShaderProgram.h"
#include "Materials.h"


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 Uv;
};

class Mesh{
private:
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    Material m_mat;
    void init();

public:

    Mesh(
            std::vector<Vertex> vertices, 
            std::vector<unsigned int> indices, 
            Material mat
        ) :
            m_vertices{vertices},
            m_indices{indices},
            m_mat{mat} 
        {
            init();
        }

    void draw(ShaderProgram &shader) const;
            
};
