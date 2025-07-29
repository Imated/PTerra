#include "Player.h"

#include "GLFW/glfw3.h"
#include "glm/ext/quaternion_geometric.hpp"
#include "misc/Constants.h"
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

    void Player::update(Window* window, float deltaTime) {
        auto direction = glm::vec2(0);
        direction.x += glfwGetKey(window->getWindow(), GLFW_KEY_D) == GLFW_PRESS;
        direction.x -= glfwGetKey(window->getWindow(), GLFW_KEY_A) == GLFW_PRESS;
        direction.y += glfwGetKey(window->getWindow(), GLFW_KEY_W) == GLFW_PRESS;
        direction.y -= glfwGetKey(window->getWindow(), GLFW_KEY_S) == GLFW_PRESS;

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
