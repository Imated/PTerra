#pragma once
#include <memory>

#include "Camera.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Terra {
    class Renderer
    {
    public:
        ~Renderer();

        static void initialize(Window *window);
        static void renderQuad();
        static Camera* getCamera();

    private:
        static inline GLuint quadVAO;
        static inline GLuint quadVBO;
        static inline std::unique_ptr<Camera> camera;
    };
}
