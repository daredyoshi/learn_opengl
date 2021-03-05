#version 330 core
struct PointLight{
    vec3 position;   
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linear;
    float quadratic;
};

out vec4 FragColor;

uniform PointLight light;


void main()
{
    FragColor = vec4(light.diffuse, 1.0);
}
