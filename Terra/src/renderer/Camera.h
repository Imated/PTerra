#pragma once
#include "Window.h"
#include "glm/mat4x4.hpp"

namespace Terra {
class Camera {
    public:
    Camera(Window *window);
    ~Camera();

    glm::mat4x4 getViewMatrix();
    glm::mat4x4 getProjectionMatrix();
    void setPosition(glm::vec2 position);
    glm::vec2 getPosition();
    glm::ivec2 getChunk();
    glm::ivec2 getChunkCentered();

    private:
    Window::WindowParams windowSize;
    glm::vec2 cameraPos;
};
}
