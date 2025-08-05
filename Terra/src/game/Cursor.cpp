#include "Cursor.h"
#include "glfw/glfw3.h"
#include "glm/fwd.hpp"
#include "glm/vec4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/gtc/type_ptr.inl"
#include "misc/Constants.h"
#include "misc/Utility.h"
#include "renderer/Renderer.h"
#include "renderer/ShaderLibrary.h"
#include "world/Tile.h"
#include "world/World.h"

namespace Terra {
    void Cursor::init() {
        position = glm::vec3(0.0f);
        texture = new Texture("resources/cursor.png");
        shader = ShaderLibrary::get("cursor").get();
    }

    void Cursor::update(Window* window, float deltaTime) {
        if (Utils::isClicked(window, GLFW_MOUSE_BUTTON_LEFT)) {
            World::setGlobalTileAt(position, std::make_unique<Tile>(1, position));
            World::getGlobalTileAt(position)->onPlace();
        }

        double cursorX, cursorY;
        int windowWidth, windowHeight;
        glfwGetWindowSize(window->getWindow(), &windowWidth, &windowHeight);
        glfwGetCursorPos(window->getWindow(), &cursorX, &cursorY);
        auto u = cursorX / windowWidth;
        float v = static_cast<float>(windowHeight - cursorY) / static_cast<float>(windowHeight);
        glm::vec2 camBL = Renderer::getCamera()->getPosition();
        float worldX = camBL.x + u * HORIZONTAL_TILES;
        float worldY = camBL.y + v * VERTICAL_TILES;
        position = glm::ivec3(std::floor(worldX), std::floor(worldY), 0.0f);

        animationTimer += deltaTime;

        if (animationTimer >= 0.25f) {
            currentFrame++;
            currentFrame = currentFrame % 2;
            animationTimer = 0.0f;
        }
    }

    void Cursor::render(glm::mat4 vp) {
        shader->use();
        texture->bind(2);
        shader->setInt ("frame", currentFrame);
        shader->setInt ("cursorAtlas", 2);
        auto mvp = glm::translate(vp, glm::vec3(position, LAYER_BEHIND_PLAYER));
        shader->setMatrix4x4("mvp", glm::value_ptr(mvp));
        Renderer::renderQuad();
    }
}
