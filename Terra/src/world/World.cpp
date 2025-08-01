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

        if (!Utils::fileExists("data/regions"))
            Utils::createDirectory("data/regions");

        // initialize the 4 regions around camera when first starting
        /*chunkData::createRegionFile({0, 0});
        chunkData::createRegionFile({-1, 0});
        chunkData::createRegionFile({0, -1});
        chunkData::createRegionFile({-1, -1});*/
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
            }
        }

        return std::make_unique<Chunk>(
            chunkPos,
            std::move(chunkArray)
        );
    }

    // ngl even idk how ts works
    Tile* World::getGlobalTileAt(glm::ivec2 worldPos) {
        glm::ivec2 chunkPos = glm::floor(glm::vec2(worldPos) / glm::vec2(CHUNK_WIDTH, CHUNK_HEIGHT));
        auto it = worldChunks.find(chunkPos);
        if (it == worldChunks.end()) {
            //DEBUG("Null chunk got for chunk pos %i %i", chunkPos.x, chunkPos.y);
            return nullptr;
        }

        Chunk* chunk = it->second.get();

        glm::ivec2 tilePos = {
            (worldPos.x % CHUNK_WIDTH + CHUNK_WIDTH) % CHUNK_WIDTH,
            (worldPos.y % CHUNK_HEIGHT + CHUNK_HEIGHT) % CHUNK_HEIGHT
        };

        return chunk->getTileAt(tilePos);
    }

    // load chunk at pos around center chunk (camera chunk is center chunk, so load chunk around camera)
    void World::chunkData::loadChunk(glm::ivec2 localPos, glm::ivec2 centerChunk) {
        auto pos = localPos + centerChunk; // world chunk position
        auto it = worldChunks.find(pos); // try to get the chunk at world pos.
        if (it != worldChunks.end()) { // if chunk found then set the chunks array which is an array that stores chunks that need to be rendered and updated
            chunks[localPos.x + MAX_CHUNKS_X/2][localPos.y + MAX_CHUNKS_Y/2] = it->second.get();
        } else {
            WARN("Tried to load chunk that doesn't exist: %d, %d", pos.x, pos.y);
        }
    }

    // return an array of chunks from disk based on the region position that was passed in
    std::vector<std::unique_ptr<Chunk>> World::chunkData::loadChunksFromDisk(glm::ivec2 regionPos) {
        std::string regionFileName = "data/regions/region_" + std::to_string(regionPos.x) + "_" + std::to_string(regionPos.y) + ".dat";
        std::string region = Utils::readFile(regionFileName.c_str());

        std::vector<std::unique_ptr<Chunk>> chunks;
        size_t offset = 0;

        for (int x = 0; x < REGION_X; ++x) {
            for (int y = 0; y < REGION_Y; ++y) {
                int16_t chunkX;
                int16_t chunkY;
                std::memcpy(&chunkX, region.data() + offset, sizeof(int16_t)); offset += sizeof(int16_t); // copy the data from disk into the chunkX int and offset so it reads the next peice of data
                std::memcpy(&chunkY, region.data() + offset, sizeof(int16_t)); offset += sizeof(int16_t);

                //DEBUG("chunk pos %i %i for region %i %i", chunkX, chunkY, regionPos.x, regionPos.y);
                //throw std::runtime_error("eee");

                std::array<std::array<std::unique_ptr<Tile>, CHUNK_WIDTH>, CHUNK_HEIGHT> tiles;

                for (int tileX = 0; tileX < CHUNK_WIDTH; ++tileX) {
                    for (int tileY = 0; tileY < CHUNK_HEIGHT; ++tileY) {
                        bool tileType;
                        uint16_t tileId;
                        std::memcpy(&tileType, region.data() + offset, sizeof(bool)); offset += sizeof(bool);
                        std::memcpy(&tileId, region.data() + offset, sizeof(uint16_t)); offset += sizeof(uint16_t);
                        if (tileType == TILE_TYPE_AUTO_TILE) // create tile based on tileID and type found in disk
                            tiles[tileX][tileY] = std::make_unique<AutoTile>(tileId, glm::ivec2(chunkX*CHUNK_WIDTH+tileX, chunkY*CHUNK_HEIGHT+tileY));
                        else
                            tiles[tileX][tileY] = std::make_unique<Tile>(tileId, glm::ivec2(chunkX*CHUNK_WIDTH+tileX, chunkY*CHUNK_HEIGHT+tileY));
                    }
                }

                chunks.push_back(std::make_unique<Chunk>(glm::ivec2(chunkX, chunkY), std::move(tiles))); // tiles is an array of unique_ptr's and unique_ptrs cant be copied so we use std::move to move them into the chunk
            }
        }

        //DEBUG("Region %i %i contains chunks (size of %i) which contains: ", regionPos.x, regionPos.y, chunks.size());
        //for (std::unique_ptr<Chunk>& chunk : chunks) {
        //    DEBUG("  - %i %i (first tile %i %i)", chunk->chunkPos.x, chunk->chunkPos.y, chunk->tiles[0][0]->pos.x, chunk->tiles[0][0]->pos.y);
        //}
        // throw std::runtime_error("eeee");

        return chunks;
    }

    // will generate chunks and create a region file for them
    void World::chunkData::createRegionFile(glm::ivec2 regionPos) {
        std::string regionFileName = "data/regions/region_" + std::to_string(regionPos.x) + "_" + std::to_string(regionPos.y) + ".dat";
        std::string regionData;
        // cx and cy are the chunk offsets within the region
        for (int cx = 0; cx < REGION_X; ++cx) {
            for (int cy = 0; cy < REGION_Y; ++cy) {
                glm::ivec2 worldChunkPos = regionPos * glm::ivec2(REGION_X, REGION_Y) + glm::ivec2(cx, cy); // get world chunk position based on offsets
                int16_t chunkX = worldChunkPos.x;
                int16_t chunkY = worldChunkPos.y;
                regionData.append(reinterpret_cast<const char*>(&chunkX), sizeof(chunkX)); // add the chunk positions into the region file data
                regionData.append(reinterpret_cast<const char*>(&chunkY), sizeof(chunkY));
                //INFO("Saving chunk at world pos: (%i, %i)", chunkX, chunkY);

                auto chunk = generateChunk(worldChunkPos); // gen chunk

                for (int tx = 0; tx < CHUNK_WIDTH; ++tx) { // tx and ty are the tile offsets within the chunk
                    for (int ty = 0; ty < CHUNK_HEIGHT; ++ty) {
                        auto tile = chunk->getTileAt({tx, ty}); // chunk should never be null bc we just generated it
                        bool type = 0;
                        if (dynamic_cast<AutoTile*>(tile) != nullptr) // if tile that we generated is an autotile then set its type to autotile type
                            type = TILE_TYPE_AUTO_TILE;
                        else
                            type = TILE_TYPE_TILE; // otherwise set it to normal tile type
                        uint16_t id = tile->getId();
                        regionData.append(reinterpret_cast<const char*>(&type), sizeof(type)); // add data to region file data
                        regionData.append(reinterpret_cast<const char*>(&id), sizeof(id));
                    }
                }
            }
        }

        Utils::writeFile(regionFileName.c_str(), regionData.data(), regionData.size()); // write data to actual region file
        INFO("Created new region file: %s", regionFileName.c_str());
    }

    void World::updateChunks() {
        glm::ivec2 camChunk = Renderer::getCamera()->getChunkCentered();

        // gets the top left and bottom right chunk thats visible
        glm::ivec2 minChunk = camChunk - glm::ivec2(MAX_CHUNKS_X / 2, MAX_CHUNKS_Y / 2);
        glm::ivec2 maxChunk = camChunk + glm::ivec2(MAX_CHUNKS_X / 2, MAX_CHUNKS_Y / 2);

        // get the top left and bottom right region positions that are visible by the camera
        glm::ivec2 minRegion = floor(glm::vec2(minChunk) / glm::vec2(REGION_X, REGION_Y));
        glm::ivec2 maxRegion = floor(glm::vec2(maxChunk) / glm::vec2(REGION_X, REGION_Y));

        // loop through and load/create all regions around camera
        for (int rx = minRegion.x; rx <= maxRegion.x; rx++) {
            for (int ry = minRegion.y; ry <= maxRegion.y; ry++) {
                glm::ivec2 region = {rx, ry};
                std::string filename = "data/regions/region_" + std::to_string(rx) + "_" + std::to_string(ry) + ".dat";
                if (!Utils::fileExists(filename.c_str())) // if there isnt a region file to load, create one
                    chunkData::createRegionFile(region);
                auto chunksToCache = chunkData::loadChunksFromDisk(region); // then load
                for (auto& chunk : chunksToCache)
                    worldChunks[chunk->chunkPos] = std::move(chunk); // add all chunks that were loaded by region file into worldChunks, again we use std::move bc its unique_ptr's that cant be copied and only moved
            }
        }

        //unload chunks that are more than 48 chunks away, 48 bc 4 region files loaded is 32x32 + 8 chunk buffer on each side (could reduce if uses too much memory)
        int halfSize = 24;

        // set variable "it" to the beginning of worldChunks. while it is not equal to the end, loop through and check if chunk is more than 48 chunks away and if it is then erase it, its like a for loop kinda
        for (auto it = worldChunks.begin(); it != worldChunks.end(); ) {
            glm::ivec2 pos = it->first;
            if (std::abs(pos.x - camChunk.x) > halfSize || std::abs(pos.y - camChunk.y) > halfSize)
                it = worldChunks.erase(it);
            else
                ++it;
        }

        // unload all chunks before loading
        for (int x = 0; x < MAX_CHUNKS_X; x++) {
            for (int y = 0; y < MAX_CHUNKS_Y; y++) {
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
                        //if (chunk->getTileAt({x, y})->getId() == 1)
                        //    DEBUG("updating tile at %i %i", chunk->chunkPos.x+x, chunk->chunkPos.y+y);
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
                    DEBUG("Chunk is null");
            }
        }
    }
}
