#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float xOffset;
out vec3 vertexColor; // output a color to the fragment shader
out vec3 newPos;

void main()
{
    // aPos.x = aPos.x + xOffset;
    vec3 newPos = aPos;
    newPos+= xOffset;

    gl_Position = vec4(newPos, 1.0);
    vertexColor = newPos; 
}
