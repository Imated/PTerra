#include "Camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "misc/Constants.h"

namespace Terra {
    Camera::Camera(Window* window): windowSize(window->params), cameraPos() {}

    Camera::~Camera() {}

    glm::mat4x4 Camera::getViewMatrix() {
        glm::mat4x4 view = glm::mat4x4(1.0f);
        view = glm::translate(view, glm::vec3(-cameraPos, 0));
        return view;
    }

    glm::mat4x4 Camera::getProjectionMatrix() {
        glm::mat4x4 projection = glm::mat4x4(1.0f);
        projection = glm::ortho(0.f, HORIZONTAL_TILES, 0.f, VERTICAL_TILES);
        return projection;
    }

    void Camera::setPosition(glm::vec2 position) {
        cameraPos = position;
    }

    glm::vec2 Camera::getPosition() {
        return cameraPos;
    }
}
