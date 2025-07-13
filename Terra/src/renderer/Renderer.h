#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Terra {
    class Renderer
    {
    public:
        ~Renderer();

        static void initialize();
        static void renderQuad(float width, float height);
        static void setClearColor(const glm::vec4& color);

    private:
        static GLuint quadVAO;
        static GLuint quadVBO;
    };
}
