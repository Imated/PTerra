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

#include "AutoTile.h"
#include "WorldHelper.h"
#include "renderer/Camera.h"

namespace Terra {
    World::World(): tileAtlas("resources/tileAtlas.png"), tileShader(nullptr) {
        seed = Random::get<int32_t>(INT32_MIN, INT32_MAX);
        DEBUG("Random seed: %i", seed);
    }

    World::~World() = default;

    void World::init() {
        auto shaderPtr = ShaderLibrary::get("default");
        if (!shaderPtr) {
            ERR("Shader 'default' not loaded!");
            return;
        }
        tileShader = shaderPtr.get();
        tileShader->setInt("mainTexture", 0);
    }

    void World::loadChunk(glm::ivec2 pos) {
        WorldHelper::loadedChunks.push_back(generateChunk(pos));
    }

    void World::unloadChunk(glm::ivec2 pos) {
        if (const auto it = WorldHelper::getChunkAt(pos); it != WorldHelper::loadedChunks.end()) {
            WorldHelper::loadedChunks.erase(it);
        }
    }

    void World::updateChunks() {
        glm::vec2 camPos = Renderer::getCamera()->getPosition();
        glm::ivec2 camChunk = { std::floor(camPos.x / CHUNK_WIDTH), std::floor(camPos.y / CHUNK_WIDTH) };

        std::vector<glm::ivec2> chunkPosNeeded;

        for (int x = 0; x < MAX_CHUNKS_X; x++) {
            for (int y = 0; y < MAX_CHUNKS_Y; y++) {
                const glm::ivec2 worldPos = camChunk + glm::ivec2(x, y);
                if (WorldHelper::getChunkAt(worldPos) == WorldHelper::loadedChunks.end())
                    loadChunk(worldPos);
                chunkPosNeeded.emplace_back(worldPos);
            }
        }

        std::vector<glm::ivec2> chunksToUnload;

        for (const auto& chunk : WorldHelper::loadedChunks) {
            auto it = std::ranges::find_if(chunkPosNeeded, [&](const glm::ivec2 &chunkPos) {
                return chunkPos == chunk.chunkPos;
            });
            if (it == chunkPosNeeded.end()) {
                chunksToUnload.push_back(chunk.chunkPos);
            }
        }

        for (const auto& pos : chunksToUnload) {
            unloadChunk(pos);
        }
    }

    Chunk World::generateChunk(glm::vec2 chunkPos) {
        std::array<std::array<std::unique_ptr<Tile>, 16>, 16> chunkArray;
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                if (x != 0 && y != 0 && x != 15 && y != 15)
                    chunkArray[x][y] = std::make_unique<AutoTile>(1, glm::ivec2(x, y));
                else
                    chunkArray[x][y] = std::make_unique<AutoTile>(0, glm::ivec2(x, y));
            }
        }

        return Chunk {
            chunkPos,
            std::move(chunkArray)
        };
    }

    void World::render(glm::mat4 vp) {
        tileShader->use();
        tileAtlas.bind(0);
        for (Chunk& chunk: WorldHelper::loadedChunks) {
            chunk.render(vp, tileShader);
        }
    }
}
