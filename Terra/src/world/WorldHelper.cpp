#include "WorldHelper.h"

#include <cmath>

#include "misc/Constants.h"

namespace Terra {
    std::vector<Chunk>::iterator WorldHelper::getChunkAt(glm::ivec2 pos) {
        const auto it = std::ranges::find_if(loadedChunks, [&](const Chunk &chunk) {
              return chunk.chunkPos == pos;
          });
        return it;
    }

    Tile* WorldHelper::getTileAt(glm::vec2 pos) {
        glm::ivec2 chunkPos = glm::ivec2(floor(pos.x / CHUNK_WIDTH), floor(pos.y / CHUNK_HEIGHT));

        glm::ivec2 tilePos = glm::ivec2(
            static_cast<int>(std::floor(pos.x)) % CHUNK_WIDTH,
            static_cast<int>(std::floor(pos.y)) % CHUNK_HEIGHT
        );

        const auto chunk = getChunkAt(chunkPos);
        if (chunk == loadedChunks.end())
            return nullptr;

        return chunk->getTileAt(tilePos);
    }
}
