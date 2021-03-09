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
    std::vector<Vertex> m_vertices {};
    std::vector<unsigned int> m_indices {};
    Material m_mat;

public:

    Mesh(
            Material mat
        ) :
            m_mat{mat} 
        {
            init();
        }

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

    void init();
    void draw(ShaderProgram &shader) const;
    void setVertices(std::vector<Vertex> vertices){ m_vertices = vertices;}
    void setIndices(std::vector<unsigned int> indices){ m_indices= indices;}
            
};



class DepthTestMesh : public Mesh{
public:
    DepthTestMesh(Material mat) : Mesh(mat) {
        std::vector<Vertex> vertices {}; 
        std::vector<unsigned int> indices {};
        unsigned int numVertices = m_positions.size() / 3;
        
        vertices.reserve(numVertices);
        indices.reserve(numVertices);
        for (unsigned int i{0}; i < numVertices; ++i){
            float x = m_positions[i*3];
            float y = m_positions[i*3 + 1];
            float z = m_positions[i*3 + 2];

            float u = m_uvs[i*2];
            float v = m_positions[i*2 + 1];
            Vertex vtx {
                glm::vec3{x, y, z},
                glm::vec3{0.0, 0.0, 0.0}, // no normals avaliable
                glm::vec2{u, v}
            };
            vertices.push_back(vtx);
            indices.push_back(i);
        }
        Mesh::setVertices(vertices);
        Mesh::setIndices(indices);
        Mesh::init();
    }

private:
    const std::vector<float> m_positions = {{
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
    }};

    const std::vector<float> m_uvs = {{
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
                   
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
                   
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
                   
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
                   
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
                   
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    }};
};













