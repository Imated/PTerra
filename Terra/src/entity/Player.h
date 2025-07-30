#pragma once
#include "Entity.h"
#include "renderer/Camera.h"

namespace Terra {
    class Player : public Entity {
    public:
        void init() override;
        void handleKeys(Window *window);
        void update(Window *window, float deltaTime) override;
    private:
        Camera* camera = nullptr;
        glm::vec2 direction = glm::vec2(0.0f);
    };
}

