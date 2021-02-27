#include <vector>
#include <array>
#include <string>

#include "glm/glm.hpp"

#include "ShaderProgram.h"
#include "Texture.h"


// A Geometry template class 
// The first vertex attrib pointer is the position, second uv, third color
template <unsigned int size> // T is the number of vertices in this geo
class Geometry{
private:
    unsigned int m_VBO;
    unsigned int m_VAO;
    const ShaderProgram &m_shader;
    std::array<float, size*3> m_vertices;
    std::array<float, size*2> m_uvs;
    std::array<float, size*3> m_vertexCols;

    std::vector<Texture> m_textures;

    bool m_hasUvs{false};
    bool m_hasVertexCols{false};


    // this generates vertex buffer, array, and vertex attrib pointers
    void setVertexData(){
        // bind the vertex array object for editing
        glBindVertexArray(m_VAO);

        // create an array with all the vertex data
        // first all the positions (required)
        // second all the uvs (optional)
        // third all the vertex colors (optional)
        //
        // determin the length of the array
        unsigned int vaoStep = 3;
        if(m_hasUvs){
            vaoStep +=2;
        }
        if(m_hasVertexCols){
            vaoStep+= 3;
        }
        int vaoLen = vaoStep*size;
        
        std::vector<float> vertexData(vaoLen);
        

        for(unsigned int i{0}; i<size; ++i){

            for(unsigned int a{0}; a<3; ++a){
                vertexData[(i*vaoStep)+a] = m_vertices[(i*3)+a];
            }
            int stepInc = 3;
            if(m_hasUvs){
                for(unsigned int u{0}; u<2; ++u){
                    vertexData[(i*vaoStep+stepInc)+u] = m_uvs[(i*2)+u];
                }
                stepInc = 5;
            }
            if(m_hasVertexCols){
                for(unsigned int c{0}; c<3; ++c){
                    vertexData[(i*vaoStep+stepInc)+c] = m_vertexCols[(i*3)+c];
                }
            }

        }
        for(float f : vertexData){
            std::cout << f << '\n';

        }
        // to edit this object we need to bind the id to openGL
        // you can bind one object of each type at a time before having to unbind
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        // now any buffer calls we make (on the GL_ARRAY_BUFFER target) will
        // be used to configure the currently bound buffer
        // now bind our vertices to the array buffer
        // This function allocates memory and stores the data within the 
        // curerntly bound buffer object
        // There are three ways that this function can treat the data
        //      GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times
        //      GL_STATIC_DRAW: the data is set only once and used many times
        //      GL_DYNAMIC_DRAW: the data is changed a lot and used manytimes
        std::cout << "vdata size = " << vertexData.size() << '\n';
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData.front(), GL_STATIC_DRAW);
       
        // We will now tell OpenGL how it should interpret the vertex data (per attribute)
        //
        //
        // The first param specifies which vertex attribute we want to configure. (location = 0)
        // The next argument specifies the size of the attribute
        // The next is the type of the attribute
        // The next is if we want to force normalize the data. 
        // The next is how large our stride is. This is the space between consecutive vertex attributes
        // (we know that the array is githly packed so this could also have been 0)
        // The last parameter sets the offset of where the data begins in the buffer
        // Now we are using every 6th float
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vaoStep * sizeof(float), (void*)0);
        // 0 is the intex of the vertex attrib array we want to activate
        glEnableVertexAttribArray(0);
        // for the uv coordinates
        if(m_hasUvs){ 
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vaoStep*sizeof(float), (void*)(3*sizeof(float)));
            glEnableVertexAttribArray(1);
        }
        if(m_hasVertexCols){
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vaoStep*sizeof(float), (void*)((vaoStep-3)*sizeof(float)));
            glEnableVertexAttribArray(2);
        }

    }
    void init(){
        
        std::cout << "Has Uvs? " << m_hasUvs << '\n';
        std::cout << "Has Vecols?" << m_hasVertexCols << '\n';
        // create a float array out of all the geometry attributes
        glGenVertexArrays(1, &m_VAO);

        // create our vertex buffer object. This will pass the data to the gpu
        // all objects in openGL are access via an id. 
        glGenBuffers(1, &m_VBO);
        setVertexData();  
    }


public:
    Geometry(
            const ShaderProgram &shader,  
            std::array<float, size*3> vertices, 
            std::array<float, size*2> uvs, 
            std::array<float, size*3> vertexCols, 
            std::vector<Texture> textures=std::vector<Texture>())
        :      
        m_shader{shader},
        m_vertices{vertices},
        m_uvs{uvs},
        m_vertexCols{vertexCols},
        m_textures{textures}
        {      
        m_hasUvs = true;
        m_hasVertexCols = true;
        init();
    }

    Geometry(const ShaderProgram &shader,
            std::array<float, size*3> vertices, 
            std::array<float, size*2> uvs, 
            std::vector<Texture> textures=std::vector<Texture>()):
        m_shader{shader},
        m_vertices{vertices},
        m_uvs{uvs},
        m_textures{textures}
    {
        m_hasUvs = true;
        init();
    }

    Geometry(
            const ShaderProgram &shader,  
            std::array<float, size*3> vertices, 
            std::vector<Texture> textures=std::vector<Texture>()):
        m_shader{shader},
        m_vertices{vertices},
        m_textures{textures}
    {
        init();
    }

    
    ~Geometry(){
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    };

    void draw(){ 
        // draw the textures of the object
        for(size_t i = 0; i < m_textures.size(); ++i){
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        }
        glBindVertexArray(m_VAO);
        // not sure 
        glDrawArrays(GL_TRIANGLES, 0, size);

    }
        
};


