#pragma once
#include <vector>

#include "Chunk.h"

namespace Terra {
class WorldHelper {
public:
    static std::vector<Chunk>::iterator getChunkAt(glm::ivec2 pos);
    static Tile* getTileAt(glm::vec2 pos);
    static inline std::vector<Chunk> loadedChunks;
};
}
