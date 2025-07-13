#pragma once
#include <cstring>

#include "Tile.h"
#include "glm/vec2.hpp"

namespace Terra {
    struct Chunk {
        glm::vec2 chunkPos {};
        char tiles[16][16] {};

        Chunk() = default;

        Chunk(glm::vec2 chunkPos, char tiles[16][16]) {
            this->chunkPos = chunkPos;
            std::memcpy(this->tiles, tiles, sizeof(this->tiles));;
        }
    };
}
