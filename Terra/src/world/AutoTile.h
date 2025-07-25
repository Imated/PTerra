#pragma once
#include "Tile.h"

namespace Terra {
    class AutoTile : public Tile {
    public:
        explicit AutoTile(uint8_t id = 0, glm::ivec2 pos = glm::ivec2(0));
        void onPlace() override;
        void update();
    private:
        uint8_t hgfedcba = 0;
    };
}
