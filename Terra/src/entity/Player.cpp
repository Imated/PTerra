#include "Player.h"

#include "GLFW/glfw3.h"
#include "glm/ext/quaternion_geometric.hpp"
#include "misc/Constants.h"
#include "misc/Utility.h"
#include "renderer/Renderer.h"
#include "renderer/ShaderLibrary.h"
#include "renderer/Window.h"

namespace Terra {
    void Player::init() {
        position = glm::vec2(0, 0);
        size = glm::vec2(1, 1);
        atlas = new Texture("resources/playerAtlas.png");
        shader = ShaderLibrary::get("player").get();
        camera = Renderer::getCamera();
        Entity::init();
    }

    void Player::handleKeys(Window *window) {
        direction.x = static_cast<float>(Utils::isPressed(window, GLFW_KEY_D) - Utils::isPressed(window, GLFW_KEY_A));
        direction.y = static_cast<float>(Utils::isPressed(window, GLFW_KEY_W) - Utils::isPressed(window, GLFW_KEY_S));
    }

    void Player::update(Window* window, float deltaTime) {
        handleKeys(window);
        if (direction != glm::vec2(0)) {
            direction = glm::normalize(glm::vec2(direction));
            currentAnimation = 1;
        }
        else
            currentAnimation = 0;

        auto movementSpeed = 32.f;
        position += direction * glm::vec2(deltaTime) * movementSpeed;
        camera->setPosition(glm::vec2(position.x - HORIZONTAL_TILES / 2, position.y - VERTICAL_TILES / 2));
        Entity::update(window, deltaTime);
    }
}
