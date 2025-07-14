#pragma once
#include <cstring>

#include "Tile.h"
#include "glm/vec2.hpp"

namespace Terra {
    struct Chunk {
        glm::ivec2 chunkPos {};
        Tile tiles[16][16] {};

        Chunk() = default;

        Chunk(glm::ivec2 chunkPos, Tile tilesIn[16][16]) {
            this->chunkPos = chunkPos;
            for (int x = 0; x < 16; ++x)
                for (int y = 0; y < 16; ++y)
                    this->tiles[x][y] = tilesIn[x][y];
        }
    };
}
