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

    void Container::render(glm::mat4 vp) {
        shader->use();
        containerTexture->bind(3);
        shader->setInt("tex", 3);
        auto mvp = glm::translate(vp, glm::vec3(0, 0, LAYER_FOREGROUND));
        mvp = glm::scale(mvp, glm::vec3(10.0f, 10.0f, 1.0f));
        shader->setMatrix4x4("mvp", value_ptr(mvp));
        Renderer::renderQuad();

    }
}
