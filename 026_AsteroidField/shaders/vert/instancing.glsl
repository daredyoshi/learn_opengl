#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aN;
layout (location = 2) in vec2 aUv;
layout (location = 3) in mat4 instanceMatrix;

out vec2 Uv;
out vec3 N;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;


void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * instanceMatrix)));
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
    Uv = aUv;
    N = normalMatrix*aN;
    // this is the world position of the frag pos
    FragPos = vec3(instanceMatrix* vec4(aPos, 1.0));
}
