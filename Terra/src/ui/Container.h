#pragma once
#include "glm/fwd.hpp"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

namespace Terra {
    class Window;

    class Container {
    public:
        Container();
        virtual ~Container();

        virtual void init();
        virtual void update(Window* window);
        virtual void render(glm::mat4 p);
    protected:
        Texture* containerTexture;
        bool isOpen;
    private:
        Shader* shader;
    };
}
