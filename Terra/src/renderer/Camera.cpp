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
        glm::mat4x4 projection = glm::ortho(0.f, HORIZONTAL_TILES, 0.f, VERTICAL_TILES);
        return projection;
    }

    void Camera::setPosition(glm::vec2 position) {
        cameraPos = position;
    }

    glm::vec2 Camera::getPosition() {
        return cameraPos;
    }

    glm::ivec2 Camera::getChunk() {
        glm::vec2 bl = cameraPos;
        // find screen center
        glm::vec2 center = bl + glm::vec2(HORIZONTAL_TILES/2.f, VERTICAL_TILES/2.f);
        // convert to chunk coords
        glm::vec2 f = center / glm::vec2(CHUNK_WIDTH/2, CHUNK_HEIGHT/2);
        //DEBUG("Camera chunk position: %f, %f, %f, %f",std::floor(f.x), std::floor(f.y), cameraPos.x, cameraPos.y);
        return {std::floor(f.x), std::floor(f.y)};
    }
}
