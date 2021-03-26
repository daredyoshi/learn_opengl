#pragma once

#include <functional>
#include <numeric>
#include <vector>

#include "glad/glad.h"
// glm
#include "glm/gtc/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/transform.hpp"
#define GLM_SWIZZLE
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/trigonometric.hpp"

using intList = std::vector<int>;


class Mesh{
private:
    unsigned int m_VBO;
    unsigned int m_EBO;
    unsigned int m_VAO;
    
    const intList m_vertexAttribSizes;
    const intList m_indices;
    const std::vector<float> m_vertexAttribs;

public:

    /* The vertexAttribs need to be interwoven
     * P, N, UV, C, P, N, UV, C ... 
     * Apparently that is faster to read from gpu
     */
    
    Mesh(
            intList const &vertexAttribSizes, 
            intList const &indices, 
            std::vector<float> const &vertexAttribs
            ):
                m_vertexAttribSizes{vertexAttribSizes}, 
                m_indices{indices}, 
                m_vertexAttribs{vertexAttribs}
    {
        unsigned int vertexFloatCount = std::accumulate(
                vertexAttribs.begin(), vertexAttribs.end(), 1);
        
        // the indices should all be ov triangles
        assert(indices.size() % 3 == 0);
        // the veretx attribs need to be divisible by the vertexFloatCount otherwise
        // something went wrong
        assert(vertexAttribs.size() % vertexFloatCount == 0);
        // the vertex attribs are the sizes times the indices
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        glGenVertexArrays(1, &m_VAO);

        // create and bind buffers
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexAttribs.size() * sizeof(float), 
                &vertexAttribs.front(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), 
                &indices.front(), GL_STATIC_DRAW);

        // create and enable the vertex attrib arrays of buffer
        unsigned int count{0};
        for(int vertexAttribSize : vertexAttribSizes){
            glVertexAttribPointer(0, vertexAttribSize, GL_FLOAT, GL_FALSE, 
                    vertexFloatCount*sizeof(float), (void*)(count*sizeof(float)));
            glEnableVertexAttribArray(count);
            ++count;
        }
    }

    void draw() const {
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    }

    ~Mesh(){
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }
};



