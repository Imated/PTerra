#include "Texture.h"

#include "misc/Logger.h"
#include "stb_image.h"

namespace Terra {
    Texture::Texture(const char* filename, GLint wrapMode, GLint filterMode)
    {
        int nrChannels;
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load(filename, &width, &height, &nrChannels, STBI_rgb_alpha);
        if (data == nullptr)
            ERR("Couldn't load texture!");
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        INFO("Atlas loaded: %d x %d", width, height);
    }

    Texture::~Texture()
    {
        stbi_image_free(data);
    }

    void Texture::bind(GLuint slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, handle);
    }

}
