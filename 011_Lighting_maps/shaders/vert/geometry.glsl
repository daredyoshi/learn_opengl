#version 330 core
struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aN;

out vec3 vertexColor; // output a color to the fragment shader
out vec2 Uv;
out vec3 N;
out vec3 FragPos;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;
uniform Light light;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Uv = aUv;
    N = normalMatrix*aN;
    // this is the world position of the frag pos
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    LightPos = vec3(view * vec4(light.position, 1.0)); 
}
