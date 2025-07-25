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

    static Tile* getTileAt(glm::vec2 pos);

    void loadChunk(glm::ivec2 pos);
    void unloadChunk(glm::ivec2 pos);

    void updateChunks();

    Chunk generateChunk(glm::vec2 chunkPos);

    void render(glm::mat4 vp);

private:
    Texture tileAtlas;
    int32_t seed = 0;
    Shader* defaultShader;
};
}

