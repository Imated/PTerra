#include "Hotbar.h"

#include "misc/Utility.h"

namespace Terra {
    Hotbar::Hotbar() {
        containerTexture = new Texture("resources/hotbar.png");
        isOpen = true;
    }

    void Hotbar::update(Window* window) {
        Container::update(window);
        if (Utils::isPressed(window, GLFW_KEY_E))
            isOpen = !isOpen;
    }
}
