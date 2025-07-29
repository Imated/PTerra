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

        //initial world gen
        for (int i = -16; i < 16; i++) {
            for (int j = -16; j < 16; j++) {
                auto tmpvec = glm::ivec2(i, j);
                worldData::worldChunks[tmpvec] = std::make_unique<Chunk>(generateChunk(tmpvec));
                }
            }
        }

    Chunk World::generateChunk(glm::ivec2 chunkPos) {
        INFO("Generating chunk: %i, %i", chunkPos.x, chunkPos.y);

        glm::ivec2 worldBase = chunkPos * glm::ivec2(CHUNK_WIDTH, CHUNK_HEIGHT);
        std::array<std::array<std::unique_ptr<Tile>, 16>, 16> chunkArray;
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                if (x != y)
                    chunkArray[x][y] = std::make_unique<AutoTile>(1, worldBase + glm::ivec2(x, y));
                else {
                    chunkArray[x][y] = std::make_unique<Tile>(0, worldBase + glm::ivec2(x, y));
                }
            }
        }

        INFO("Generated");

        return Chunk {
            chunkPos,
            std::move(chunkArray)
        };
    }

    Tile* World::getGlobalTileAt(glm::ivec2 worldPos) {
        glm::ivec2 chunkPos = glm::floor(glm::vec2(worldPos) / glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT));

        INFO("Getting tile at: (%i, %i) - (%i, %i)", worldPos.x, worldPos.y, chunkPos.x, chunkPos.y);

        if (std::abs(chunkPos.x) > MAX_CHUNKS_X / 2 || std::abs(chunkPos.y) > MAX_CHUNKS_Y / 2) {
            WARN("OOB chunk got., %i, %i", chunkPos.x, chunkPos.y);
            return nullptr;
        }

        auto& chunk = chunkData::chunks[chunkPos.x + MAX_CHUNKS_X / 2][chunkPos.y + MAX_CHUNKS_Y / 2];
        if (!chunk) {
            WARN("Null chunk got.");
            return nullptr;
        }

        glm::ivec2 tilePos = {
            (worldPos.x % CHUNK_WIDTH + CHUNK_WIDTH) % CHUNK_WIDTH,
            (worldPos.y % CHUNK_HEIGHT + CHUNK_HEIGHT) % CHUNK_HEIGHT
        };

        return chunk->getTileAt(tilePos);
    }

    void World::chunkData::loadChunk(glm::ivec2 localPos, glm::ivec2 centerChunk) {
        auto pos = localPos + centerChunk;
        INFO("Loading chunk %i, %i", pos.x, pos.y);
        auto it = worldData::worldChunks.find(pos);
        if (it != worldData::worldChunks.end()) {
            chunks[localPos.x + MAX_CHUNKS_X/2][localPos.y + MAX_CHUNKS_Y/2] = it->second.get();
        } else {
            WARN("Tried to load chunk that doesn't exist: %d, %d", pos.x, pos.y);
        }
    }

    void World::updateChunks() {

        glm::ivec2 camChunk = Renderer::getCamera()->getChunk();
        INFO("Camera chunk: %i, %i", camChunk.x, camChunk.y);

        for (int x = 0; x < MAX_CHUNKS_X; x++) {
            for (int y = 0; y < MAX_CHUNKS_Y; y++) {
                chunkData::chunks[x][y] = nullptr;
            }
        }

        for (int x = -(MAX_CHUNKS_X / 2); x < MAX_CHUNKS_X / 2; x++) {
            for (int y = -(MAX_CHUNKS_Y / 2); y < MAX_CHUNKS_Y / 2; y++) {
                chunkData::loadChunk({x, y}, camChunk);
            }
        }

        for (auto& chunks: chunkData::chunks) {
            for (auto& chunk: chunks) {
                if (!chunk) continue;
                for (int x = 0; x < CHUNK_WIDTH; x++) {
                    for (int y = 0; y < CHUNK_HEIGHT; y++) {
                        chunk->getTileAt({x, y})->update();
                    }
                }
            }
        }
    }

    void World::render(glm::mat4 vp) {
        tileShader->use();
        tileAtlas.bind(0);
        for (const auto &chunks : chunkData::chunks) {
            for (const auto &chunk : chunks) {
                if (chunk != nullptr)
                    chunk->render(vp, tileShader);
                else
                    WARN("Chunk is null");
            }
        }
    }
}
