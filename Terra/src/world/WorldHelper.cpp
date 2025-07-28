#include "WorldHelper.h"

#include <cmath>
#include <ranges>

#include "AutoTile.h"
#include "World.h"
#include "glm/common.hpp"
#include "misc/Constants.h"
#include "misc/Logger.h"

namespace Terra {
    Chunk WorldHelper::generateChunk(glm::vec2 chunkPos) {
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

        INFO("Generated");

        return Chunk {
            chunkPos,
            std::move(chunkArray)
        };
    }

    Tile* WorldHelper::getTileAt(glm::vec2 worldPos) {
        //DEBUG("Searching for tile at: %f, %f", worldPos.x, worldPos.y);
        const glm::ivec2 chunkPos = glm::floor(worldPos / glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT));
        const auto chunk = loadedChunks.find(chunkPos);
        if (chunk == loadedChunks.end())
            return nullptr;
        //INFO("Found for tile at: %f, %f", glm::mod(worldPos, glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT)).x, glm::mod(worldPos, glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT)).y);

        return chunk->second->getTileAt(glm::mod(worldPos, glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT)));
    }
}
