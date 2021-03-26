#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

// this class should not be called directly, rather one of the 
// model type classes should be used. 
class Model{
private:
    std::vector<Mesh> m_meshes;
public:
    void addMesh(const Mesh mesh){ m_meshes.push_back(mesh); }
    void draw(ShaderProgram &shader) const;
};


class AssimpModel : public Model{
private:
    std::string m_directory;
public:
    AssimpModel(std::string_view modelFile){
        loadModel(modelFile);
    }
    void loadModel(std::string_view path);
    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
    Texture loadMaterialTextures(aiMaterial *mat, 
            aiTextureType type, unsigned int idx);
};


