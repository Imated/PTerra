#pragma once
#include "Entity.h"
#include "renderer/Camera.h"

namespace Terra {
    class Player : public Entity {
    public:
        void init() override;
        void update(Window *window, float deltaTime) override;
    private:
        Camera* camera = nullptr;
    };
}

