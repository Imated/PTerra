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

    static Tile* getGlobalTileAt(glm::ivec2 worldPos, bool top = false);
    static void setGlobalTileAt(glm::ivec2 worldPos, std::unique_ptr<Tile> tile, bool top = false);
    static std::unique_ptr<Chunk> generateChunk(glm::ivec2 chunkPos);

    struct chunkData {
        static inline std::array<std::array<Chunk*,MAX_CHUNKS_X>, MAX_CHUNKS_Y> chunks; // chunks that are actively updated and rendered
        static void loadChunk(glm::ivec2 worldPos, glm::ivec2 centerChunk);
        static std::vector<std::unique_ptr<Chunk>> loadChunksFromDisk(glm::ivec2 regionPos);
        static void createRegionFile(glm::ivec2 regionPos);
    };
    static chunkData loadedChunks;

    struct IVec2Hasher {
        std::size_t operator()(const glm::ivec2& v) const {
            return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
        }
    };

    static inline std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, IVec2Hasher> worldChunks; // stores 4 region files aka 32x32 grid of chunks around the camera

private:
    Texture tileAtlas;
    int32_t seed = 0;
    Shader* tileShader;
};
}

