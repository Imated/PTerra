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

    static std::vector<Tile*> tilesToUpdate;

    void World::loadChunk(glm::ivec2 pos) {
        Chunk newChunk = WorldHelper::generateChunk(pos);
        WorldHelper::loadedChunks.push_back(std::move(newChunk));

        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                tilesToUpdate.push_back(WorldHelper::loadedChunks.back().getTileAt({x, y}));
            }
        }
    }

    void World::unloadChunk(glm::ivec2 pos) {
        if (const auto it = WorldHelper::getChunkAt(pos); it != WorldHelper::loadedChunks.end()) {
            WorldHelper::loadedChunks.erase(it);
        }
    }

    void World::updateChunks() {
        glm::ivec2 camChunk = Renderer::getCamera()->getChunk();

        std::vector<glm::ivec2> chunkPosNeeded;

        for (int x = -(MAX_CHUNKS_X / 2); x <= MAX_CHUNKS_X / 2; x++) {
            for (int y = -(MAX_CHUNKS_Y / 2); y <= MAX_CHUNKS_Y / 2; y++) {
                const glm::ivec2 worldPos = camChunk + glm::ivec2(x, y);
                auto it = WorldHelper::getChunkAt(worldPos);
                if (it == WorldHelper::loadedChunks.end()) {
                    loadChunk(worldPos);
                }

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

        for (const auto& tile : tilesToUpdate) {
            tile->update();
        }
        tilesToUpdate.clear();
    }

    void World::render(glm::mat4 vp) {
        tileShader->use();
        tileAtlas.bind(0);
        for (Chunk& chunk : WorldHelper::loadedChunks) {
            chunk.render(vp, tileShader);
        }
    }
}
