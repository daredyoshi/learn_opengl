#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// this won't compile because I forgot to give my cube array Normals
out vec3 N;
out vec3 FragPos;
uniform mat3 normalMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords= aTexCoords;
    N = normalMatrix*aN;
    // this is the world position of the frag pos
    FragPos = vec3(model * vec4(aPos, 1.0));
}
