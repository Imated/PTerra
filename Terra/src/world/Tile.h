#pragma once
#include <vector>

#include "Registry.h"
#include "glm/vec2.hpp"

namespace Terra {
    struct TileData {
        uint8_t frameCount;
        std::vector<uint8_t> stateLocations;
    };

    class Tile {
        public:
        virtual ~Tile() = default;
        explicit Tile(uint8_t id = 0, glm::ivec2 pos = glm::ivec2(0));

        TileData* getTileData();
        virtual void onPlace();
        virtual void update();

        uint8_t getId();
        uint8_t getFrame();

        protected:
        uint8_t id;
        glm::ivec2 pos {};
        uint8_t currentFrame = 0;
    };
}
