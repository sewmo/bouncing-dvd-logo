#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 vTexCoord;

uniform float xPos;
uniform float yPos;

void main() 
{
    gl_Position = vec4(aPos.x + xPos, aPos.y + yPos, aPos.z, 1.0f);
    vTexCoord = aTexCoord;
}