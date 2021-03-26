#version 330 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 Uv;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform bool flip;

void main()
{
    vec2 newUv = Uv;
    if(flip){
        newUv.x = 1 - newUv.x;
    }
    FragColor = mix(texture(texture1, Uv), texture(texture2, newUv), 0.5);
}
