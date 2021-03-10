#version 330 core
in vec2 TexCoords;
in vec3 FragPos;


in vec3 N;

uniform sampler2D texture1;

uniform vec3 cameraPos;
uniform samplerCube skybox;

out vec4 FragColor;

void main()
{             
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(N), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
  
}


