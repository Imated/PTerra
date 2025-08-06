#include "Container.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"
#include "misc/Constants.h"
#include "renderer/Renderer.h"
#include "renderer/ShaderLibrary.h"

namespace Terra {
    Container::Container() : containerTexture(nullptr), shader(nullptr) {
    }

    Container::~Container() = default;

    void Container::init() {
        shader = ShaderLibrary::get("default").get();
    }

    void Container::update(Window* window) {

    }

    void Container::render(glm::mat4 p) {
        if (!isOpen)
            return;
        shader->use();
        containerTexture->bind(3);
        shader->setInt("tex", 3);
        auto textureHalfWidth = containerTexture->getWidth() / 2;
        auto textureHalfWidthTiles = textureHalfWidth / 16;
        auto mvp = glm::translate(p, glm::vec3(HORIZONTAL_TILES / 2 - textureHalfWidthTiles, 0.25f, LAYER_FOREGROUND));
        float textureAspect = static_cast<float>(containerTexture->getHeight()) / static_cast<float>(containerTexture->getWidth());
        mvp = glm::scale(mvp, glm::vec3(12.0f, 12.0f * textureAspect, 1.0f));
        shader->setMatrix4x4("mvp", value_ptr(mvp));
        Renderer::renderQuad();

    }
}
