#pragma once
#include <memory>
#include <unordered_map>

#include "Chunk.h"
#include "misc/Constants.h"
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

namespace Terra {

class World {
public:
    World();
    ~World();

    void init();

    void updateChunks();

    void render(glm::mat4 vp);

    static Tile* getGlobalTileAt(glm::ivec2 worldPos);
    static Chunk generateChunk(glm::ivec2 chunkPos);

    struct chunkData { //struct providing storage and methods for loading and unloading chunks
        static inline std::array<std::array<Chunk*,MAX_CHUNKS_X>, MAX_CHUNKS_Y> chunks; //chunks actively updated and rendered
        static void loadChunk(glm::ivec2 worldPos, glm::ivec2 centerChunk);
        static void unloadChunk(glm::ivec2 worldPos);
    };
    static chunkData loadedChunks;

    struct IVec2Hasher {
        std::size_t operator()(const glm::ivec2& v) const {
            return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
        }
    };

    struct IVec2Equal {
        bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
            return a.x == b.x && a.y == b.y;
        }
    };

    struct worldData {
        static inline std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, IVec2Hasher, IVec2Equal> worldChunks;
    };

private:
    Texture tileAtlas;
    int32_t seed = 0;
    Shader* tileShader;
};
}

