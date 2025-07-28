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

    Chunk* World::worldData::operator[](glm::ivec2 worldPos) {
        if (worldPos.x >= 0 && worldPos.y >= 0) { //top right I
            return &World::worldData::quadrant1[worldPos.x][worldPos.y];
        }
        if (worldPos.x < 0 && worldPos.y >= 0) { //top left II
            return &World::worldData::quadrant2[-worldPos.x][worldPos.y];
        }
        if (worldPos.x < 0 && worldPos.y < 0) { //bottom left III
            return &World::worldData::quadrant3[-worldPos.x][-worldPos.y];
        }
        if (worldPos.x >= 0 && worldPos.y < 0) { //bottom right IIII
            return &World::worldData::quadrant4[worldPos.x][-worldPos.y];
        }
        ERR("How the fuck.");
        return nullptr;
    }

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

        World::worldData::quadrant1 = new std::array<Chunk, 64>[64];
        World::worldData::quadrant2 = new std::array<Chunk, 64>[64];
        World::worldData::quadrant3 = new std::array<Chunk, 64>[64];
        World::worldData::quadrant4 = new std::array<Chunk, 64>[64];

        //initial world gen
        for (int i = -16; i < 16; i++) {
            for (int j = -16; j < 16; j++) {
                auto tmpvec = glm::ivec2(i, j);
                *world[tmpvec] = World::generateChunk(tmpvec);
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
        //DEBUG("Searching for tile at: %f, %f", worldPos.x, worldPos.y);
        const glm::ivec2 chunkPos = glm::floor(intBitsToFloat(worldPos) / glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT));
        const auto chunk = chunkData::chunks[chunkPos.x][chunkPos.y];
        //INFO("Found for tile at: %f, %f", glm::mod(worldPos, glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT)).x, glm::mod(worldPos, glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT)).y);

        return chunk->getTileAt(worldPos % glm::ivec2(CHUNK_WIDTH, CHUNK_HEIGHT));
    }

    static std::vector<Tile*> tilesToUpdate;

    void World::chunkData::loadChunk(glm::ivec2 worldPos, glm::ivec2 centerChunk) {
        glm::ivec2 pos = {centerChunk.x + worldPos.x, centerChunk.y + worldPos.y};
        World::chunkData::chunks[pos.x + MAX_CHUNKS_X/2][pos.y + MAX_CHUNKS_Y/2] = World::world[worldPos];
        World::chunkData::globalPositions[std::vector<int>{worldPos.x, worldPos.y}] = pos;

        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                tilesToUpdate.push_back(World::chunkData::chunks[pos.x][pos.y]->getTileAt({x, y}));
            }
        }
    }

    void World::chunkData::unloadChunk(glm::ivec2 worldPos) {
        World::chunkData::chunks[World::chunkData::globalPositions[std::vector<int>{worldPos.x, worldPos.y}].x][World::chunkData::globalPositions[std::vector<int>{worldPos.x, worldPos.y}].y] = nullptr;
        World::chunkData::globalPositions.erase(std::vector<int>{worldPos.x, worldPos.y});
    }

    void World::updateChunks() {

        glm::ivec2 camChunk = Renderer::getCamera()->getChunk();
        INFO("Camera chunk: %i, %i", camChunk.x, camChunk.y);

        for (int x = 0; x < MAX_CHUNKS_X; x++) {
            for (int y = 0; y < MAX_CHUNKS_Y; y++) {
                World::chunkData::chunks[x][y] = nullptr;
            }
        }
        World::chunkData::globalPositions.clear();

        for (int x = -(MAX_CHUNKS_X / 2); x <= MAX_CHUNKS_X / 2; x++) {
            for (int y = -(MAX_CHUNKS_Y / 2); y <= MAX_CHUNKS_Y / 2; y++) {
                const glm::ivec2 worldPos = camChunk + glm::ivec2(x, y);
                INFO("Camera chunk: %i, %i, %i, %i", worldPos.x, worldPos.y, camChunk.x, camChunk.y);
                World::chunkData::loadChunk(worldPos, camChunk);
            }
        }

        /*
        std::vector<glm::ivec2> chunksToUnload;

        for (const auto& pos: WorldHelper::loadedChunks | std::views::keys) {
            if (std::ranges::find(chunkPosNeeded, pos) == chunkPosNeeded.end()) {
                chunksToUnload.emplace_back(pos);
            }
        }

        for (const auto& pos : chunksToUnload) {
            unloadChunk(pos);
        }
        */

        for (const auto& tile : tilesToUpdate) {
            tile->update();
        }
        tilesToUpdate.clear();
    }

    void World::render(glm::mat4 vp) {
        tileShader->use();
        tileAtlas.bind(0);
        for (const auto &chunks: World::chunkData::chunks) {
            for (const auto &chunk : chunks) {
                chunk->render(vp, tileShader);
            }
        }
    }
}
