#version 330 core

out vec4 FragColor;
in vec2 Uv;
in vec3 N;
in vec3 FragPos;

uniform bool flip;
uniform float scale;
uniform float opacity;

uniform vec3 cameraPos;
uniform samplerCube skybox;


struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuseTex1;
    sampler2D diffuseTex2;
    sampler2D diffuseTex3;
    sampler2D specularTex;
};


uniform Material material;



void main()
{

    vec3 I  = normalize(FragPos - cameraPos);
    vec3 R = reflect(I, normalize(N));
    vec3 reflection = vec3(texture(skybox, R));
    float specular = float(texture(material.specularTex, Uv));
    vec3 diffuse = vec3(texture(material.diffuseTex1, Uv));
    vec3 result = mix(diffuse, reflection, specular); 
    FragColor = vec4(result, 1.0);
}

