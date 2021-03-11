#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aN;
layout (location = 2) in vec2 aUv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;

out VS_OUT {
    vec2 Uv;
    vec3 N;
    vec3 FragPos;
} vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.Uv = aUv;
    vs_out.N = normalMatrix*aN;
    // this is the world position of the frag pos
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
}
