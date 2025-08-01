#include "Player.h"

#include "audio/Audio.h"
#include "GLFW/glfw3.h"
#include "glm/ext/quaternion_geometric.hpp"
#include "misc/Constants.h"
#include "misc/Random.h"
#include "misc/Utility.h"
#include "renderer/Renderer.h"
#include "renderer/ShaderLibrary.h"
#include "renderer/Window.h"

namespace Terra {
    void Player::init() {
        position = glm::vec2(0, 400);
        size = glm::vec2(1, 1);
        atlas = new Texture("resources/playerAtlas.png");
        shader = ShaderLibrary::get("player").get();
        camera = Renderer::getCamera();
        Audio::loadSound("playerGrassFootsteps", "resources/footstepsGrass.wav", false);
        footstepSource = Audio::playSound("playerGrassFootsteps", 1.f, 1.f, {}, {}, true);
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
            footstepSource->setPosition(position);
            footstepSource->setPitch(Random::get<float>(0.8f, 1.2f));
            footstepSource->setGain(Random::get<float>(0.7f, 1.0f));
            if (!footstepSource->isPlaying())
                footstepSource->resume();
        }
        else {
            currentAnimation = 0;
            footstepSource->pause();
        }

        auto movementSpeed = 32.f;
        position += direction * glm::vec2(deltaTime) * movementSpeed;
        camera->setPosition(glm::vec2(position.x - HORIZONTAL_TILES / 2, position.y - VERTICAL_TILES / 2));
        Entity::update(window, deltaTime);
    }
}
