#pragma once
#include <array>
#include <memory>

#include "Tile.h"
#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "renderer/Shader.h"

namespace Terra {
    class Chunk {
    public:
        glm::ivec2 chunkPos {};
        std::array<std::array<std::unique_ptr<Tile>, 16>, 16> groundTiles;
        std::array<std::array<std::unique_ptr<Tile>, 16>, 16> topTiles;

        Chunk() {
            for (int x = 0; x < 16; ++x)
                for (int y = 0; y < 16; ++y) {
                    groundTiles[x][y] = nullptr;
                    topTiles[x][y] = nullptr;
                }
        }

        Chunk(glm::ivec2 chunkPos,
            std::array<std::array<std::unique_ptr<Tile>, 16>, 16> groundTilesIn
        );
        Chunk(glm::ivec2 chunkPos,
            std::array<std::array<std::unique_ptr<Tile>, 16>, 16> groundTilesIn,
            std::array<std::array<std::unique_ptr<Tile>, 16>, 16> topTilesIn
        );

        ~Chunk();

        void render(glm::mat4 vp, Shader* shader);

        Tile* getTileAt(glm::ivec2 pos, bool top = false) const;
        void setTileAt(glm::ivec2 pos, std::unique_ptr<Tile> tile, bool top = false);
    };
}
