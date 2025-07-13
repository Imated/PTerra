#pragma once
#include "Chunk.h"
#include "renderer/Renderer.h"
#include "renderer/Texture.h"

namespace Terra {

class World {
public:
    World();
    ~World();

    void generateChunks();

    Chunk generateChunk(glm::vec2 chunkPos);

private:
    Texture tileAtlas;
    Chunk loadedChunks[16][16];
    uint32_t seed = 0;
    Renderer renderer;
};

}

