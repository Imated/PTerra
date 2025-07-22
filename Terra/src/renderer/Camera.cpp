#include "Camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Terra {
    Camera::Camera(Window* window, uint8_t defaultZoom): windowSize(window->params), zoom(defaultZoom), cameraPos() {}

    Camera::~Camera() {}

    glm::mat4x4 Camera::getViewMatrix() {
        glm::mat4x4 view = glm::mat4x4(1.0f);
        view = glm::translate(view, glm::vec3(-cameraPos, 0));
        return view;
    }

    glm::mat4x4 Camera::getProjectionMatrix() {
        glm::mat4x4 projection = glm::mat4x4(1.0f);
        float halfW = windowSize.width / zoom;
        float halfH = windowSize.height / zoom;
        projection = glm::ortho(-halfW, halfW, -halfH, halfH, -1.0f, 1.0f);
        return projection;
    }

    void Camera::setPosition(glm::vec2 position) {
        this->cameraPos = position;
    }

    glm::vec2 Camera::getPosition() {
        return cameraPos;
    }
}
