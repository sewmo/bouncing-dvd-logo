#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

enum TextureFormat {
    PNG,
    JPG
};

class Texture
{
public:
    GLuint ID;

    Texture(const char* texturePath, TextureFormat format, bool flipTexture);

};

#endif