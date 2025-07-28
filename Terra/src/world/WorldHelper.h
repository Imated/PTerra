#pragma once
#include <optional>
#include <vector>

#include "Chunk.h"
#include "misc/Constants.h"

namespace Terra {
class WorldHelper {
private:
    struct IVec2Hash {
        std::size_t operator()(const glm::ivec2& v) const noexcept {
            return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
        }
    };
public:
    static Tile* getTileAt(glm::vec2 worldPos);
    static Chunk generateChunk(glm::vec2 chunkPos);
    static inline std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, IVec2Hash> loadedChunks;
};
}
