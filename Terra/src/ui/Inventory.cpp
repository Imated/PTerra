#include "Inventory.h"

#include "misc/Utility.h"

namespace Terra {
    Inventory::Inventory() {
        containerTexture = new Texture("resources/inventory.png");
        isOpen = false;
    }

    void Inventory::update(Window* window) {
        Container::update(window);
        if (Utils::isPressed(window, GLFW_KEY_E) == GLFW_PRESS && !wasPressed) {
            isOpen = !isOpen;
            wasPressed = true;
        }
        else if (Utils::isPressed(window, GLFW_KEY_E) == GLFW_RELEASE && wasPressed) {
            wasPressed = false;
        }
    }
}
