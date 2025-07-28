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

    static void loadChunk(glm::ivec2 worldPos);
    static void unloadChunk(glm::ivec2 pos);

    void updateChunks();

    void render(glm::mat4 vp);

private:
    Texture tileAtlas;
    int32_t seed = 0;
    Shader* tileShader;
};
}

