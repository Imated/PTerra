#include "World.h"

#include <ctime>
#include <memory_resource>
#include <random>

#include "Chunk.h"
#include "renderer/ShaderLibrary.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Registry.h"
#include "misc/Logger.h"
#include "misc/Random.h"

namespace Terra {

    constexpr int CHUNK_WIDTH = 16;
    constexpr int CHUNK_HEIGHT = 16;
    constexpr int TILE_WIDTH = 16;
    constexpr int TILE_HEIGHT = 16;
    constexpr int MAX_CHUNKS_X = 4;
    constexpr int MAX_CHUNKS_Y = 3;

    World::World(): tileAtlas(RESOURCES_PATH "tileAtlas.png") {
        seed = Random::get<int32_t>(INT32_MIN, INT32_MAX);
        DEBUG("Random seed: %i", seed);
        defaultShader = ShaderLibrary::get("default").get();
        loadedChunks.resize(MAX_CHUNKS_X * MAX_CHUNKS_Y);
    }

    World::~World() = default;

    void World::generateChunks() {
        int index = 0;
        for (int x = 0; x < MAX_CHUNKS_X; x++) {
            for (int y = 0; y < MAX_CHUNKS_Y; y++) {
                loadedChunks[index++] = generateChunk(glm::vec2(x, y));
            }
        }
    }

    Chunk World::generateChunk(glm::vec2 chunkPos) {
        Tile chunkArray[16][16] {};
        for (auto& x : chunkArray) {
            for (auto& y : x) {
                y = Tile(0);
            }
        }

        chunkArray[8][8] = Tile(1);

        return Chunk {
            chunkPos,
            chunkArray
        };
    }

    void World::render() {
        float halfW = 1920 / (4.0f);
        float halfH = 1080 / (4.0f);
        defaultShader->activate();
        tileAtlas.bind();

        for (Chunk& chunk: loadedChunks) {
            for (int x = 0; x < CHUNK_WIDTH; ++x) {
                for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                    glm::vec2 offset = glm::vec2(
                        -MAX_CHUNKS_X * CHUNK_WIDTH * TILE_WIDTH / 2.0f,
                        -MAX_CHUNKS_Y * CHUNK_HEIGHT * TILE_HEIGHT / 2.0f
                    );

                    glm::vec2 tilePos = glm::vec2(
                        (chunk.chunkPos.x * CHUNK_WIDTH + x) * TILE_WIDTH,
                        (chunk.chunkPos.y * CHUNK_HEIGHT + y) * TILE_HEIGHT
                    ) + offset;

                    glm::mat4 mvp = glm::ortho(-halfW, halfW, -halfH, halfH, -1.0f, 1.0f)
                                    * glm::translate(glm::mat4(1.0f), glm::vec3(tilePos, 0.0f));
                    defaultShader->setMatrix4x4("mvp", value_ptr(mvp));

                    defaultShader->setInt("tileID", chunk.tiles[x][y].getTileData()->startFrame);

                    Renderer::renderQuad(TILE_WIDTH, TILE_HEIGHT);
                }
            }
        }
    }
}
