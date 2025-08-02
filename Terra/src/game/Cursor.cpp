#include "Cursor.h"
#include "glfw/glfw3.h"
#include "glm/fwd.hpp"
#include "glm/vec4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/gtc/type_ptr.inl"
#include "misc/Constants.h"
#include "renderer/Renderer.h"
#include "renderer/ShaderLibrary.h"

namespace Terra {
    void Cursor::init() {
        position = glm::vec3(0.0f);
        texture = new Texture("resources/cursor.png");
        shader = ShaderLibrary::get("cursor").get();
    }

    void Cursor::render(Window* window, glm::mat4 vp, float deltaTime) {
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

        if (animationTimer >= 0.5f) {
            currentFrame++;
            currentFrame = currentFrame % 2;
            animationTimer = 0.0f;
        }

        shader->use();
        texture->bind(2);
        shader->setInt ("frame", currentFrame);
        shader->setInt ("cursorAtlas", 2);
        auto mvp = glm::translate(vp, glm::vec3(position, LAYER_BEHIND_PLAYER));
        shader->setMatrix4x4("mvp", glm::value_ptr(mvp));
        Renderer::renderQuad();
    }
}
