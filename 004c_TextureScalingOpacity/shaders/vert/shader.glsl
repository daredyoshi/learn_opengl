#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUv;

out vec3 vertexColor; // output a color to the fragment shader
out vec2 Uv;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor; 
    Uv = aUv;
}
