#pragma once
#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/Window.h"

namespace Terra {
    class Cursor {
    public:
        void init();
        void update(Window* window, float deltaTime);
        void render(glm::mat4 vp);
    private:
        glm::vec2 position = { };
        Texture* texture = nullptr;
        Shader* shader = nullptr;
        int currentFrame = 0;
        float animationTimer = 0.f;
    };
}
