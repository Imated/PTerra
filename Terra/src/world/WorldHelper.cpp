#include "WorldHelper.h"

#include <cmath>

#include "AutoTile.h"
#include "AutoTile.h"
#include "AutoTile.h"
#include "World.h"
#include "glm/common.hpp"
#include "misc/Constants.h"
#include "misc/Logger.h"

namespace Terra {
    Chunk WorldHelper::generateChunk(glm::vec2 chunkPos) {
        if (getChunkAt(chunkPos) != loadedChunks.end()) {
            WARN("Chunk already loaded.");
            return Chunk();
        }
        DEBUG("Generating chunk: %f, %f", chunkPos.x, chunkPos.y);

        glm::ivec2 worldBase = chunkPos * glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT);
        std::array<std::array<std::unique_ptr<Tile>, 16>, 16> chunkArray;
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                if (x != y)
                    chunkArray[x][y] = std::make_unique<AutoTile>(1, worldBase + glm::ivec2(x, y));
                else {
                    chunkArray[x][y] = std::make_unique<Tile>(0, worldBase + glm::ivec2(x, y));
                }
            }
        }

        return Chunk {
            chunkPos,
            std::move(chunkArray)
        };
    }

    std::vector<Chunk>::iterator WorldHelper::getChunkAt(glm::ivec2 pos) {
        const auto it = std::ranges::find_if(loadedChunks, [&](const Chunk &chunk) {
              return chunk.chunkPos == pos;
          });
        return it;
    }

    Tile* WorldHelper::getTileAt(glm::vec2 worldPos) {
        DEBUG("Searching for tile at: %f, %f", worldPos.x, worldPos.y);
        glm::ivec2 chunkPos = glm::floor(worldPos / glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT));
        auto chunk = getChunkAt(chunkPos);
        if (chunk == loadedChunks.end()) {
            //World::loadChunk(chunkPos);
            chunk = getChunkAt(chunkPos);
            ERR("Failed to find tile");
            return nullptr;
        }

        INFO("Found for tile at: %f, %f", chunk->getTileAt(glm::mod(worldPos, glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT)))->pos.x, chunk->getTileAt(glm::mod(worldPos, glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT)))->pos.y);

        return chunk->getTileAt(glm::mod(worldPos, glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT)));
    }
}
