#include "Mesh.h"

#include "glm/glm.hpp"




class Renderable{
private:
    // this will get generated from inputs
    const Mesh& m_mesh;
    // the model xform that this stores
    glm::mat4& m_model;

public:
    Renderable(const Mesh& mesh, glm::mat4& model):
        m_mesh{mesh}, m_model{model} {} 

    void setTransform(const glm::mat4& model){m_model = model;}
    //
    const glm::mat4& getTransform() const {return m_model;}

    void draw() const{
        m_mesh.draw();
    }
};



