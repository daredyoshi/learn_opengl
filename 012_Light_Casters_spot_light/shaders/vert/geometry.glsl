#version 330 core
struct PointLight{
    vec3 position;   
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linear;
    float quadratic;
    float cutoff;
    float outerCutoff;
};


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aN;

out vec3 vertexColor; // output a color to the fragment shader
out vec2 Uv;
out vec3 N;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;
uniform PointLight light;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Uv = aUv;
    N = normalMatrix*aN;
    // this is the world position of the frag pos
    FragPos = vec3(model * vec4(aPos, 1.0));
}