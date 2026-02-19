#version 330 core
out vec4 FragColor;
in vec2 vTexCoord;

uniform sampler2D textureSampler;

void main() 
{
    // FragColor = texture(ourTexture, vTexCoord) * vec4(vColor, 1.0);
    FragColor = texture(textureSampler, vTexCoord);
}