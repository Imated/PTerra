#include "WorldHelper.h"

namespace Terra {
    std::vector<Chunk>::iterator WorldHelper::getChunkAt(glm::ivec2 pos) {
        const auto it = std::ranges::find_if(loadedChunks, [&](const Chunk &chunk) {
              return chunk.chunkPos == pos;
          });
        return it;
    }
}