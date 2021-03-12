#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model{
private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;

    void loadModel(std::string_view path);
    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
    Texture loadMaterialTextures(aiMaterial *mat, 
            aiTextureType type, unsigned int idx);

public:
    Model(std::string_view modelFile){
        loadModel(modelFile);
    }
    void draw(ShaderProgram &shader) const;
    std::vector<Mesh>& getMeshes() { return m_meshes; }
};
