#version 330 core
out vec4 FragColor;
in vec2 Uv;
in vec3 vertexColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform bool flip;
uniform float scale;
uniform float opacity;

uniform vec3 lightColor;

void main()
{
    vec2 newUv = Uv;
    if(flip){
        newUv.x = 1 - newUv.x;
    }
    /* newUv *= scale; */
    vec4 diffuse = mix(texture(texture1, Uv), texture(texture2, newUv), opacity);
    /* FragColor = diffuse * vec4(lightColor, 1.0); */
    FragColor = diffuse;
}
