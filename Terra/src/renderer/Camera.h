#pragma once
#include "Window.h"
#include "glm/mat4x4.hpp"

namespace Terra {
class Camera {
    public:
    Camera(Window *window, uint8_t defaultZoom);
    ~Camera();

    glm::mat4x4 getViewMatrix();
    glm::mat4x4 getProjectionMatrix();
    void setPosition(glm::vec2 position);
    glm::vec2 getPosition();

    private:
    Window::WindowParams windowSize;
    uint8_t zoom;
    glm::vec2 cameraPos;
};
}
