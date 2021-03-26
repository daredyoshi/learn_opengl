#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aN;
layout (location = 2) in vec2 aUv;

out vec2 Uv;
out vec3 N;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;


void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Uv = aUv;
    N = normalMatrix*aN;
    // this is the world position of the frag pos
    FragPos = vec3(model* vec4(aPos, 1.0));
}
