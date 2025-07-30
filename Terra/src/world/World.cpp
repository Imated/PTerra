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
#include "misc/Utility.h"
#include "renderer/Camera.h"

namespace Terra {

    World::World(): tileAtlas("resources/tileAtlas.png"), tileShader(nullptr) {
        seed = Random::get<int32_t>(INT32_MIN, INT32_MAX);
        DEBUG("Random seed: %i", seed);
    }

    World::~World() = default;


    void World::init() {
        auto shaderPtr = ShaderLibrary::get("tile");
        if (!shaderPtr) {
            ERR("Shader 'default' not loaded!");
            return;
        }
        tileShader = shaderPtr.get();
        tileShader->setInt("mainTexture", 0);

        //initial world gen    why tf is this a 64x64 square??? i wanted to be able to walk far >:C
        // for (int i = -8; i < 8; i++) {
        //     for (int j = -8; j < 8; j++) {
        //         auto tmpvec = glm::ivec2(i, j);
        //         worldChunks[tmpvec] = generateChunk(tmpvec);
        //     }
        // }
        chunkData::createRegionFile({0, 0});
        chunkData::createRegionFile({-1, 0});
        chunkData::createRegionFile({0, -1});
        chunkData::createRegionFile({-1, -1});
    }

    std::unique_ptr<Chunk> World::generateChunk(glm::ivec2 chunkPos) {
        glm::ivec2 worldBase = chunkPos * glm::ivec2(CHUNK_WIDTH, CHUNK_HEIGHT);
        std::array<std::array<std::unique_ptr<Tile>, CHUNK_HEIGHT>, CHUNK_WIDTH> chunkArray;
        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {

                if (pow(x, y)< 55)
                    chunkArray[x][y] = std::make_unique<AutoTile>(1, worldBase + glm::ivec2(x, y));
                else
                    chunkArray[x][y] = std::make_unique<Tile>(0, worldBase + glm::ivec2(x, y));

                //chunkArray[x][y] = std::make_unique<AutoTile>(1, worldBase + glm::ivec2(x, y));
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
        //INFO("trans chunk %i %i", translatedChunk.x, translatedChunk.y);
        //INFO("Getting tile at: (%i, %i) - (%i, %i)", worldPos.x, worldPos.y, chunkPos.x, chunkPos.y);

        // if (std::abs(translatedChunk.x) > MAX_CHUNKS_X / 2.0 || std::abs(translatedChunk.y) > MAX_CHUNKS_Y / 2.0) {
        //     DEBUG("OOB chunk got., %i, %i", chunkPos.x, chunkPos.y);
        //     return nullptr;
        // }
        auto& chunk = worldChunks[chunkPos];
        if (!chunk) {
            DEBUG("Null chunk got.");
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
        //INFO("Loading chunk %i, %i", pos.x, pos.y);
        auto it = worldChunks.find(pos);
        if (it != worldChunks.end()) {
            chunks[localPos.x + MAX_CHUNKS_X/2][localPos.y + MAX_CHUNKS_Y/2] = it->second.get();
        } else {
            WARN("Tried to load chunk that doesn't exist: %d, %d", pos.x, pos.y);
        }
    }

    std::vector<std::unique_ptr<Chunk>> World::chunkData::loadChunksFromDisk(glm::ivec2 regionPos) {
        std::string regionFileName = "data/regions/region_" + std::to_string(regionPos.x) + "_" + std::to_string(regionPos.y) + ".dat";
        std::string region = Utils::readFile(regionFileName.c_str());

        std::vector<std::unique_ptr<Chunk>> chunks;
        size_t offset = 0;
        auto camChunk = Renderer::getCamera()->getChunk();

        for (int x = 0; x < REGION_X; ++x) {
            for (int y = 0; y < REGION_Y; ++y) {
                int16_t chunkX;
                int16_t chunkY;
                std::memcpy(&chunkX, region.data() + offset, sizeof(int16_t)); offset += sizeof(int16_t);
                std::memcpy(&chunkY, region.data() + offset, sizeof(int16_t)); offset += sizeof(int16_t);
                //INFO("Loading chunk at world pos: (%i, %i)", chunkX, chunkY);

                std::array<std::array<std::unique_ptr<Tile>, CHUNK_WIDTH>, CHUNK_HEIGHT> tiles;

                for (int tileX = 0; tileX < CHUNK_WIDTH; ++tileX) {
                    for (int tileY = 0; tileY < CHUNK_HEIGHT; ++tileY) {
                        bool tileType;
                        uint16_t tileId;
                        std::memcpy(&tileType, region.data() + offset, sizeof(bool)); offset += sizeof(bool);
                        std::memcpy(&tileId, region.data() + offset, sizeof(uint16_t)); offset += sizeof(uint16_t);
                        if (tileType == TILE_TYPE_AUTO_TILE)
                            tiles[tileX][tileY] = std::make_unique<AutoTile>(tileId, glm::ivec2(tileX, tileY));
                        else
                            tiles[tileX][tileY] = std::make_unique<Tile>(tileId, glm::ivec2(tileX, tileY));
                    }
                }

                chunks.push_back(std::make_unique<Chunk>(glm::ivec2(chunkX, chunkY), std::move(tiles)));
            }
        }
        return chunks;
    }

    void World::chunkData::createRegionFile(glm::ivec2 regionPos) {
        std::string regionFileName = "data/regions/region_" + std::to_string(regionPos.x) + "_" + std::to_string(regionPos.y) + ".dat";
        std::string regionData;
        for (int cx = 0; cx < REGION_X; ++cx) {
            for (int cy = 0; cy < REGION_Y; ++cy) {
                glm::ivec2 worldChunkPos = regionPos * glm::ivec2(REGION_X, REGION_Y) + glm::ivec2(cx, cy);
                int16_t chunkX = worldChunkPos.x;
                int16_t chunkY = worldChunkPos.y;
                regionData.append(reinterpret_cast<const char*>(&chunkX), sizeof(chunkX));
                regionData.append(reinterpret_cast<const char*>(&chunkY), sizeof(chunkY));
                //INFO("Saving chunk at world pos: (%i, %i)", chunkX, chunkY);

                auto chunk = generateChunk(worldChunkPos);

                for (int tx = 0; tx < CHUNK_WIDTH; ++tx) {
                    for (int ty = 0; ty < CHUNK_HEIGHT; ++ty) {
                        auto tile = chunk->getTileAt({tx, ty});
                        bool type = 0;
                        if (dynamic_cast<AutoTile*>(tile) != nullptr)
                            type = TILE_TYPE_AUTO_TILE;
                        else
                            type = TILE_TYPE_TILE;
                        uint16_t id = tile->getId();
                        regionData.append(reinterpret_cast<const char*>(&type), sizeof(type));
                        regionData.append(reinterpret_cast<const char*>(&id), sizeof(id));
                    }
                }
            }
        }

        Utils::writeFile(regionFileName.c_str(), regionData.data(), regionData.size());
        INFO("Created new region file: %s", regionFileName.c_str());
    }

    void World::updateChunks() {
        glm::ivec2 camChunk = Renderer::getCamera()->getChunkCentered();
        glm::ivec2 regionPos = floor(glm::vec2(camChunk) / glm::vec2(REGION_X, REGION_Y));
        glm::ivec2 regionBase = regionPos * glm::ivec2(REGION_X);
        glm::ivec2 regionEnd = regionBase + glm::ivec2(REGION_Y - 1);

        // if near edge of a region, load the next region in
        glm::ivec2 minChunk = camChunk - glm::ivec2(MAX_CHUNKS_X / 2, MAX_CHUNKS_Y / 2);
        glm::ivec2 maxChunk = camChunk + glm::ivec2(MAX_CHUNKS_X / 2, MAX_CHUNKS_Y / 2);

        // get the region bounds covered by visible chunks
        glm::ivec2 minRegion = floor(glm::vec2(minChunk) / glm::vec2(REGION_X, REGION_Y));
        glm::ivec2 maxRegion = floor(glm::vec2(maxChunk) / glm::vec2(REGION_X, REGION_Y));

        //INFO("AAAAAAAAAAA %i, %i, %i, %i", minRegion.x, minRegion.y, maxRegion.x, maxRegion.y);
        for (int rx = minRegion.x; rx <= maxRegion.x; rx++) {
            for (int ry = minRegion.y; ry <= maxRegion.y; ry++) {
                glm::ivec2 region = {rx, ry};
                std::string filename = "data/regions/region_" + std::to_string(rx) + "_" + std::to_string(ry) + ".dat";
                if (!Utils::fileExists(filename.c_str()))
                    chunkData::createRegionFile(region);
                auto chunksToCache = chunkData::loadChunksFromDisk(region);
                for (auto& chunk : chunksToCache)
                    worldChunks[chunk->chunkPos] = std::move(chunk);
            }
        }

        //unload most of old region (most not all beacuse you are in part of this region so if you unloaded all you would unload the chunks you are in)
        int halfSize = 24;

        for (auto it = worldChunks.begin(); it != worldChunks.end(); ) {
            glm::ivec2 pos = it->first;
            if (std::abs(pos.x - camChunk.x) > halfSize || std::abs(pos.y - camChunk.y) > halfSize)
                it = worldChunks.erase(it);
            else
                ++it;
        }

        // unload all chunks before loading
        for (int x = 0; x <= MAX_CHUNKS_X; x++) {
            for (int y = 0; y <= MAX_CHUNKS_Y; y++) {
                chunkData::chunks[x][y] = nullptr;
            }
        }

        //load all chunks around camera
        for (int x = -(MAX_CHUNKS_X / 2); x <= MAX_CHUNKS_X / 2; x++) {
            for (int y = -(MAX_CHUNKS_Y / 2); y <= MAX_CHUNKS_Y / 2; y++) {
                chunkData::loadChunk({x, y}, camChunk);
            }
        }

        // update all chunks
        for (auto& chunks: chunkData::chunks) {
            for (auto& chunk: chunks) {
                if (!chunk)
                    continue;
                for (int x = 0; x < CHUNK_WIDTH; x++) {
                    for (int y = 0; y < CHUNK_HEIGHT; y++) {
                        chunk->getTileAt({x, y})->update();
                       // INFO("Updating tile %d,%d with ID %d", x, y, chunk->getTileAt({x, y})->getId());
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
                    DEBUG("Chunk is null");
            }
        }
    }
}
