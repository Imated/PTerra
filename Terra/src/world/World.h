#pragma once
#include <memory>
#include <unordered_map>

#include "Chunk.h"
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

namespace Terra {

class World {
public:
    World();
    ~World();

    void init();

    void loadChunk(glm::ivec2 pos);
    void unloadChunk(glm::ivec2 pos);

    bool isChunkLoaded(glm::ivec2 chunkPos);
    void updateChunks();
    void generateChunks();

    Chunk generateChunk(glm::vec2 chunkPos);

    void render(glm::mat4 vp);

private:
    Texture tileAtlas;
    std::vector<Chunk> loadedChunks;
    int32_t seed = 0;
    Shader* defaultShader;
};
}

