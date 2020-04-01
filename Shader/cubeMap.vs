#version 330 core
layout (location = 0) in vec3 aPos;

layout (location = 2) in vec2 aTexCoords;

out vec3 WorldPos;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    
    TexCoords = aTexCoords;
    
    WorldPos = aPos;
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}
