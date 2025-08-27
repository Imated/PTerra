#pragma once
#include "Container.h"

namespace Terra {
    class Window;

    class Hotbar : public Container {
    public:
        Hotbar();
        void update(Window *window) override;
    private:
        bool wasPressed = false;
    };
}
