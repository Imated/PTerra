#include "entity/Entity.h"

#include "glm/gtc/type_ptr.hpp"
#include "misc/Constants.h"
#include "renderer/Camera.h"
#include "renderer/Renderer.h"

namespace Terra {
    void Entity::render(glm::mat4 vp) const {
        shader->use();
        atlas->bind(1);
        shader->setInt ("frame", currentFrame);
        shader->setInt ("anim", currentAnimation);
        shader->setInt ("entityAtlas", 1);
        auto mvp = glm::translate(vp, glm::vec3(position, LAYER_ENTITY));
        shader->setMatrix4x4("mvp", value_ptr(mvp));
        Renderer::renderQuad();
    }

    void Entity::setPosition(glm::vec2 position) {
        this->position = position;
    }

    glm::vec2 Entity::getPosition() const {
        return position;
    }

    void Entity::setSize(glm::vec2 size) {
        this->size = size;
    }

    glm::vec2 Entity::getSize() const {
        return size;
    }

    void Entity::setFrame(uint8_t frame) {
        this->currentFrame = frame;
    }

    uint8_t Entity::getFrame() const {
        return currentFrame;
    }

    void Entity::setAnimation(uint8_t animation) {
        this->currentAnimation = animation;
    }

    uint8_t Entity::getAnimation() const {
        return currentAnimation;
    }
}
