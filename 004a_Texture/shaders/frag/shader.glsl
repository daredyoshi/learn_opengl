#version 330 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 Uv;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
    FragColor = mix(texture(texture1, Uv), texture(texture2, Uv), 0.5);
}
