//
//
//
// #include "Geometry.h"
// #include <vector>
//
//
// float vertices[] = {
//     -0.5f, -0.5f, -0.5f,  
//      0.5f, -0.5f, -0.5f,  
//      0.5f,  0.5f, -0.5f,  
//      0.5f,  0.5f, -0.5f,  
//     -0.5f,  0.5f, -0.5f,  
//     -0.5f, -0.5f, -0.5f,  
//
//     -0.5f, -0.5f,  0.5f,  
//      0.5f, -0.5f,  0.5f,  
//      0.5f,  0.5f,  0.5f,  
//      0.5f,  0.5f,  0.5f,  
//     -0.5f,  0.5f,  0.5f,  
//     -0.5f, -0.5f,  0.5f,  
//
//     -0.5f,  0.5f,  0.5f,  
//     -0.5f,  0.5f, -0.5f,  
//     -0.5f, -0.5f, -0.5f,  
//     -0.5f, -0.5f, -0.5f,  
//     -0.5f, -0.5f,  0.5f,  
//     -0.5f,  0.5f,  0.5f,  
//
//      0.5f,  0.5f,  0.5f,  
//      0.5f,  0.5f, -0.5f,  
//      0.5f, -0.5f, -0.5f,  
//      0.5f, -0.5f, -0.5f,  
//      0.5f, -0.5f,  0.5f,  
//      0.5f,  0.5f,  0.5f,  
//
//     -0.5f, -0.5f, -0.5f,  
//      0.5f, -0.5f, -0.5f,  
//      0.5f, -0.5f,  0.5f,  
//      0.5f, -0.5f,  0.5f,  
//     -0.5f, -0.5f,  0.5f,  
//     -0.5f, -0.5f, -0.5f,  
//
//     -0.5f,  0.5f, -0.5f,  
//      0.5f,  0.5f, -0.5f,  
//      0.5f,  0.5f,  0.5f,  
//      0.5f,  0.5f,  0.5f,  
//     -0.5f,  0.5f,  0.5f,  
//     -0.5f,  0.5f, -0.5f,  
// };
//
// float uvs[] = {
//     0.0f, 0.0f,
//     1.0f, 0.0f,
//     1.0f, 1.0f,
//     1.0f, 1.0f,
//     0.0f, 1.0f,
//     0.0f, 0.0f,
//                
//     0.0f, 0.0f,
//     1.0f, 0.0f,
//     1.0f, 1.0f,
//     1.0f, 1.0f,
//     0.0f, 1.0f,
//     0.0f, 0.0f,
//                
//     1.0f, 0.0f,
//     1.0f, 1.0f,
//     0.0f, 1.0f,
//     0.0f, 1.0f,
//     0.0f, 0.0f,
//     1.0f, 0.0f,
//                
//     1.0f, 0.0f,
//     1.0f, 1.0f,
//     0.0f, 1.0f,
//     0.0f, 1.0f,
//     0.0f, 0.0f,
//     1.0f, 0.0f,
//                
//     0.0f, 1.0f,
//     1.0f, 1.0f,
//     1.0f, 0.0f,
//     1.0f, 0.0f,
//     0.0f, 0.0f,
//     0.0f, 1.0f,
//                
//     0.0f, 1.0f,
//     1.0f, 1.0f,
//     1.0f, 0.0f,
//     1.0f, 0.0f,
//     0.0f, 0.0f,
//     0.0f, 1.0f
// };
//
//
// class Box : public Geometry<36> {
//
// public:
//     Box(ShaderProgram shader, std::vector<Texture> textures=std::vector<Texture>()) :
//         Geometry(shader, vertices, uvs, textures) {}
// };
