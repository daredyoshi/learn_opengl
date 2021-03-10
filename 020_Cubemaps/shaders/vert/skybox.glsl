#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main(){
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    // set the z to the w for maximum depth
    // that way the skybox is only drawn if there is nothing else there
    gl_Position = pos.xyww; 
}
