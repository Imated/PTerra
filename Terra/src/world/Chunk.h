#pragma once
#include <array>
#include <cstring>
#include <memory>

#include "Tile.h"
#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "renderer/Shader.h"

namespace Terra {
    struct Chunk {
        glm::ivec2 chunkPos {};
        std::array<std::array<std::unique_ptr<Tile>, 16>, 16> tiles;

        Chunk() {
            for (int x = 0; x < 16; ++x)
                for (int y = 0; y < 16; ++y)
                    tiles[x][y] = nullptr;
        }

        Chunk(glm::ivec2 chunkPos, std::array<std::array<std::unique_ptr<Tile>, 16>, 16> tilesIn);

        ~Chunk();

        Chunk(const Chunk&) = delete;
        Chunk& operator=(const Chunk&) = delete;

        Chunk(Chunk&&) = default;
        Chunk& operator=(Chunk&&) = default;

        void render(glm::mat4 vp, Shader* shader);

        Tile* getTileAt(glm::ivec2 pos);
    };
}
