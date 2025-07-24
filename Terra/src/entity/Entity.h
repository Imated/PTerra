#pragma once
#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/Window.h"

namespace Terra {
    class Entity {
    public:

        virtual void init() {}
        virtual ~Entity() = default;

        void render(glm::mat4 vp) const;
        virtual void update(Window *window, float deltaTime) {}

        void setPosition(glm::vec2 position);
        glm::vec2 getPosition() const;
        void setSize(glm::vec2 size);
        glm::vec2 getSize() const;
        void setFrame(uint8_t frame);
        uint8_t getFrame() const;
        void setAnimation(uint8_t animation);
        uint8_t getAnimation() const;

    protected:
        glm::vec2 position = { };
        glm::vec2 size = { };
        uint8_t currentFrame = 0;
        uint8_t currentAnimation = 0;
        Texture* atlas = nullptr;
        Shader* shader = nullptr;
    };
}
