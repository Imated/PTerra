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

    void generateChunks();

    Chunk generateChunk(glm::vec2 chunkPos);

    void render();

private:
    Texture tileAtlas;
    std::pmr::vector<Chunk> loadedChunks;
    int32_t seed = 0;
    Shader* defaultShader;
};

}

