#pragma once
#include "Container.h"

namespace Terra {
    class Inventory : public Container {
    public:
        void update(Window *window) override;
        Inventory();
    };
}
