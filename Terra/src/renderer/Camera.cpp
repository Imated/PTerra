#include "Camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

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
        float aspect = static_cast<float>(windowSize.width) / static_cast<float>(windowSize.height);
        float horizontalTiles = 60.f / 2;
        float verticalTiles = horizontalTiles / aspect;
        projection = glm::ortho(-horizontalTiles, horizontalTiles, -verticalTiles, verticalTiles, -1.0f, 1.0f);
        return projection;
    }

    void Camera::setPosition(glm::vec2 position) {
        this->cameraPos = position;
    }

    glm::vec2 Camera::getPosition() {
        return cameraPos;
    }
}
