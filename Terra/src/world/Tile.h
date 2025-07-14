#pragma once
#include <vector>

#include "Registry.h"
#include "glm/vec2.hpp"

namespace Terra {
    struct TileData {
        uint8_t startFrame;
        uint8_t frameCount;
        std::vector<uint8_t> stateLocations;
    };

    class Tile {
        public:
        Tile(uint8_t id = 0);
        TileData* getTileData();
        private:
        uint8_t id;
    };
}
