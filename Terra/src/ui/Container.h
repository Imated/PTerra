#pragma once
#include "glm/fwd.hpp"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

namespace Terra {
    class Container {
    public:
        Container();
        virtual ~Container();

        virtual void init();
        virtual void render(glm::mat4 vp);
    protected:
        Texture* containerTexture;
    private:
        Shader* shader;
    };
}
