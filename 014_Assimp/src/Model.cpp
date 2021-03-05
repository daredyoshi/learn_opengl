#include "Model.h"

std::vector<Texture> LOADED_TEXTURES;

void Model::draw(ShaderProgram &shader) const{
    for (Mesh mesh : m_meshes){
        mesh.draw(shader);
    }
}

void Model::loadModel(std::string_view path){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
            path.data(), aiProcess_Triangulate | aiProcess_FlipUVs); 
    
    // check to see if scene data might be incomplete
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
        return;
    };
    m_directory = path.substr(0, path.find_last_of('/'));
    // this will recursively build a mesh out of all the scene node's meshes
    processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode *node, const aiScene *scene){
    // process all of the nodes meshes
    for(unsigned int i{0}; i < node->mNumMeshes; ++i){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // recursvly iterate over all child nodes
    for(unsigned int i{0}; i<node->mNumChildren; ++i){
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(const aiMesh *mesh, const aiScene *scene){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    // this would be replaces with a default material
    ObsidianMaterial mat{"material"};

    for(unsigned int i{0}; i < mesh->mNumVertices; i++){
        Vertex vertex;

        // get the position
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // get the normal
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;

        // get the uv
        if(mesh->mTextureCoords[0]){
            // up to 8 different uv sets are allowed
            // but we will only take the first one
            glm::vec3 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.Uv = vec;
        }
        else
            vertex.Uv = glm::vec2();

        vertices.push_back(vertex);
    }
    // process indices
    for(unsigned int i{0}; i<mesh->mNumFaces; ++i){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j<face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }


    // this would get majorly improved for a real material pipeline
    // it's not very optimized since we're copying each texture for each mesh
        
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    // we have a maximum of 3 diffuse texture slots
    mat.setDiffuseTex1(Texture{loadMaterialTextures(material, aiTextureType_DIFFUSE, 0)});
    mat.setDiffuseTex2(Texture{loadMaterialTextures(material, aiTextureType_DIFFUSE, 1)});
    mat.setDiffuseTex3(Texture{loadMaterialTextures(material, aiTextureType_DIFFUSE, 2)});
    // right now we only have a single specular slot
    mat.setSpecularTex(Texture{loadMaterialTextures(material, aiTextureType_SPECULAR, 0)});

    return Mesh(vertices, indices, mat);
}

Texture Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, unsigned int idx){
    Texture texture{};
    unsigned int typeTextureCount{mat->GetTextureCount(type)};
    if(typeTextureCount > idx){
        aiString str;
        mat->GetTexture(type, idx, &str);
        std::string fullPath { m_directory + '/' + str.C_Str() };
        bool skip = false;
        for( Texture loadedTex : LOADED_TEXTURES ){
            std::string loadedPath = loadedTex.getPath();
            if(loadedPath.compare(fullPath)== 0){
                texture = loadedTex;
                skip = true;
                break;
            } 
        }
        if(!skip){
            std::cout << "Loading new texture " << fullPath << '\n';
            texture.load(fullPath);
            LOADED_TEXTURES.push_back(texture);
        }
    }
    return texture;
}







