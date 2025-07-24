#pragma once
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION

namespace Terra {
    class Texture {
        public:
            Texture(const char* filename, GLint wrapMode = GL_CLAMP_TO_BORDER, GLint filterMode = GL_NEAREST);
            ~Texture();

            void bind(GLuint slot);

            GLuint GetHandle() const { return handle; }
            GLint getWidth() const { return width; }
            GLint getHeight() const { return height; }
        private:
            GLuint handle = 0;
            int width = 0;
            int height = 0;
            unsigned char* data;
    };
}
