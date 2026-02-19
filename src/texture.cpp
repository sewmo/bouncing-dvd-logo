#include <texture.hpp>

Texture::Texture(const char* texturePath, TextureFormat format, bool flipTexture) 
{
    std::cout << "INFO: Constructing texture object..." << std::endl;

    stbi_set_flip_vertically_on_load(flipTexture);
    
    int width, height, nrChannels;
    unsigned char* textureData = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    if (!textureData) {
        std::cout << "ERROR::TEXTURE::COULD_NOT_LOAD_DATA" << std::endl;
    }

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (format == TextureFormat::JPG) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        std::cout << "INFO: Loaded texture with JPG format." << std::endl;
    } else if (format == TextureFormat::PNG) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        std::cout << "INFO: Loaded texture with PNG format." << std::endl;
    } else {
        std::cout << "ERROR::TEXTURE::INVALID_TEXTURE_FORMAT" << std::endl;
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(textureData);
}