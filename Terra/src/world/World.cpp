#include "World.h"

#include <ctime>
#include <memory_resource>
#include <random>

#include "Chunk.h"
#include "renderer/ShaderLibrary.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "misc/Logger.h"
#include "misc/Random.h"
#include "misc/Constants.h"
#include <algorithm>
#include <unordered_set>

namespace Terra {



    World::World(): tileAtlas(RESOURCES_PATH "tileAtlas.png") {
        seed = Random::get<int32_t>(INT32_MIN, INT32_MAX);
        DEBUG("Random seed: %i", seed);
        loadedChunks.resize(MAX_CHUNKS_X * MAX_CHUNKS_Y);
    }

    World::~World() = default;

    void World::init() {
        auto shaderPtr = ShaderLibrary::get("default");
        if (!shaderPtr) {
            ERR("Shader 'default' not loaded!");
            return;
        }

        defaultShader = shaderPtr.get();
        defaultShader->activate();
        tileAtlas.bind();
    }

    void World::loadChunk(glm::ivec2 pos) {
        loadedChunks.push_back(generateChunk(pos));
    }

    void World::unloadChunk(glm::ivec2 pos) {
        const auto it = std::ranges::find_if(loadedChunks, [&](const Chunk &chunk) {
            return chunk.chunkPos == pos;
        });

        if (it != loadedChunks.end()) {
            loadedChunks.erase(it);
        }
    }

    void World::updateChunks() {
        glm::vec2 camPos = Renderer::getCamera()->getPosition();
        glm::ivec2 camChunk = { static_cast<int>((camPos.x + 8.f) / CHUNK_WIDTH), static_cast<int>((camPos.y + 8.f) / CHUNK_WIDTH) };

        for (int x = 0; x < MAX_CHUNKS_X; x++) {
            for (int y = 0; y < MAX_CHUNKS_Y; y++) {
                const glm::ivec2 worldPos = camChunk + glm::ivec2(x, y);
                if (!isChunkLoaded(worldPos))
                    loadChunk(worldPos);
            }
        }
    }

    bool World::isChunkLoaded(glm::ivec2 pos) {
        for (const Chunk& c : loadedChunks) {
            if (c.chunkPos == pos)
                return true;
        }
        return false;
    }

    void World::generateChunks() {
        updateChunks();
    }

    Chunk World::generateChunk(glm::vec2 chunkPos) {
        Tile chunkArray[16][16] {};
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                if (x == 0 && y == 15)
                    chunkArray[x][y] = Tile(2); // top-left
                else if (x == 15 && y == 15)
                    chunkArray[x][y] = Tile(3); // top-right
                else if (x == 0 && y == 0)
                    chunkArray[x][y] = Tile(9); // bottom-left
                else if (x == 15 && y == 0)
                    chunkArray[x][y] = Tile(8); // bottom-right
                else if (y == 15)
                    chunkArray[x][y] = Tile(4); // top edge
                else if (y == 0)
                    chunkArray[x][y] = Tile(7); // bottom edge
                else if (x == 0)
                    chunkArray[x][y] = Tile(5); // left edge
                else if (x == 15)
                    chunkArray[x][y] = Tile(6); // right edge
                else
                    chunkArray[x][y] = Tile(1); // center
            }
        }

        //chunkArray[8][8] = Tile(1);

        return Chunk {
            chunkPos,
            chunkArray
        };
    }

    void World::render() {
        glm::mat4 vp = Renderer::getCamera()->getProjectionMatrix() * Renderer::getCamera()->getViewMatrix();

        for (Chunk& chunk: loadedChunks) {
            chunk.render(vp, defaultShader);
        }
    }
}
