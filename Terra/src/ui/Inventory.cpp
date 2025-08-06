#include "Inventory.h"

#include "misc/Utility.h"

namespace Terra {
    Inventory::Inventory() {
        containerTexture = new Texture("resources/inventory.png");
        isOpen = false;
    }

    void Inventory::update(Window *window) {
        if (Utils::isPressed(window, GLFW_KEY_E))
            isOpen = !isOpen;
        Container::update(window);
    }
}
