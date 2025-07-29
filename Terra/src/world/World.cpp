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

        //initial world gen    why tf is this a 64x64 square???
        for (int i = -32; i < 32; i++) {
            for (int j = -32; j < 32; j++) {
                auto tmpvec = glm::ivec2(i, j);
                worldChunks[tmpvec] = generateChunk(tmpvec);
            }
        }
    }

    std::unique_ptr<Chunk> World::generateChunk(glm::ivec2 chunkPos) {
        glm::ivec2 worldBase = chunkPos * glm::ivec2(CHUNK_WIDTH, CHUNK_HEIGHT);
        std::array<std::array<std::unique_ptr<Tile>, CHUNK_HEIGHT>, CHUNK_WIDTH> chunkArray;
        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {

                if (x + y*CHUNK_WIDTH <= chunkPos.x)
                    chunkArray[x][y] = std::make_unique<AutoTile>(1, worldBase + glm::ivec2(x, y));
                else
                    chunkArray[x][y] = std::make_unique<Tile>(0, worldBase + glm::ivec2(x, y));

            }
        }

        return std::make_unique<Chunk>(
            chunkPos,
            std::move(chunkArray)
        );
    }

    Tile* World::getGlobalTileAt(glm::ivec2 worldPos) {
        glm::ivec2 chunkPos = glm::floor(glm::vec2(worldPos) / glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT));
        glm::ivec2 translatedChunk = chunkPos - Renderer::getCamera()->getChunk();
        //INFO("Getting tile at: (%i, %i) - (%i, %i)", worldPos.x, worldPos.y, chunkPos.x, chunkPos.y);

        if (std::abs(translatedChunk.x) > MAX_CHUNKS_X / 2.0 || std::abs(translatedChunk.y) > MAX_CHUNKS_Y / 2.0) {
            //WARN("OOB chunk got., %i, %i", chunkPos.x, chunkPos.y);
            return nullptr;
        }
        auto& chunk = chunkData::chunks[translatedChunk.x + MAX_CHUNKS_X / 2][translatedChunk.y + MAX_CHUNKS_Y / 2];
        if (!chunk) {
            //WARN("Null chunk got.");
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
        auto it = worldChunks.find(pos);
        if (it != worldChunks.end()) {
            chunks[localPos.x + MAX_CHUNKS_X/2][localPos.y + MAX_CHUNKS_Y/2] = it->second.get();
        } else {
            WARN("Tried to load chunk that doesn't exist: %d, %d", pos.x, pos.y);
        }
    }

    void World::updateChunks() {

        glm::ivec2 camChunk = Renderer::getCamera()->getChunk();
        INFO("Camera chunk: %i, %i", camChunk.x, camChunk.y);

        // unload all chunks before loading
        for (int x = 0; x < MAX_CHUNKS_X; x++) {
            for (int y = 0; y < MAX_CHUNKS_Y; y++) {
                chunkData::chunks[x][y] = nullptr;
            }
        }

        //load all chunks around camera
        for (int x = -(MAX_CHUNKS_X / 2); x < MAX_CHUNKS_X / 2; x++) {
            for (int y = -(MAX_CHUNKS_Y / 2); y < MAX_CHUNKS_Y / 2; y++) {
                chunkData::loadChunk({x, y}, camChunk);
            }
        }

        // update all chunks
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
