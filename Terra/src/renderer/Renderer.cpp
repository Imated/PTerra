#include "Renderer.h"

#include <string>
#include <thread>

#include "ShaderLibrary.h"

namespace Terra {

    void Renderer::initialize(Window* window)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        camera = std::make_unique<Camera>(Camera(window));
    }

    Renderer::~Renderer()
    {
        ShaderLibrary::free("default");
    }

    void Renderer::renderQuad()
    {
        if(quadVAO == 0)
        {
            GLuint EBO;

            const auto topLeft = glm::vec2(0, 1);
            const auto topRight = glm::vec2(1, 1);
            constexpr auto bottomLeft = glm::vec2(0, 0);
            const auto bottomRight = glm::vec2(1, 0);

            GLfloat vertices[] =
            {
                // Positions                  // UV Coordinates
                topLeft.x, topLeft.y,          0.0f, 1.0f,  // Top Left
                topRight.x, topRight.y,        1.0f, 1.0f,  // Top Right
                bottomLeft.x, bottomLeft.y,    0.0f, 0.0f,  // Bottom Left
                bottomRight.x, bottomRight.y,  1.0f, 0.0f   // Bottom Right
            };

            constexpr GLuint indices[] =
            {
                0, 1, 2, // Top Left, Top Right, Bottom Left
                1, 3, 2   // Top Right, Bottom Right, Bottom Left
            };

            glGenVertexArrays(1, &quadVAO);
            glBindVertexArray(quadVAO);

            glGenBuffers(1, &quadVBO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        glBindVertexArray(quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    Camera* Renderer::getCamera() {
        return camera.get();
    }
}
