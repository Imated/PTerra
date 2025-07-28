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
#include <ranges>
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

    void World::loadChunk(glm::ivec2 worldPos) {
        Chunk newChunk = WorldHelper::generateChunk(worldPos);
        WorldHelper::loadedChunks[worldPos] = std::make_unique<Chunk>(std::move(newChunk));

        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                tilesToUpdate.push_back(WorldHelper::loadedChunks[worldPos]->getTileAt({x, y}));
            }
        }
    }

    void World::unloadChunk(glm::ivec2 worldPos) {
        WorldHelper::loadedChunks.erase(worldPos);
    }

    void World::updateChunks() {
        glm::ivec2 camChunk = Renderer::getCamera()->getChunk();
        INFO("Camera chunk: %i, %i", camChunk.x, camChunk.y);

        std::vector<glm::ivec2> chunkPosNeeded;

        for (int x = -(MAX_CHUNKS_X / 2); x <= MAX_CHUNKS_X / 2; x++) {
            for (int y = -(MAX_CHUNKS_Y / 2); y <= MAX_CHUNKS_Y / 2; y++) {
                const glm::ivec2 worldPos = camChunk + glm::ivec2(x, y);
                if (!WorldHelper::loadedChunks.contains(worldPos)) {
                    INFO("Loading chunk...");
                    loadChunk(worldPos);
                    INFO("Chunk loaded!");
                }

                chunkPosNeeded.emplace_back(worldPos);
            }
        }

        std::vector<glm::ivec2> chunksToUnload;

        for (const auto& pos: WorldHelper::loadedChunks | std::views::keys) {
            if (std::ranges::find(chunkPosNeeded, pos) == chunkPosNeeded.end()) {
                chunksToUnload.emplace_back(pos);
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
        for (const auto &chunk: WorldHelper::loadedChunks | std::views::values) {
            chunk->render(vp, tileShader);
        }
    }
}
