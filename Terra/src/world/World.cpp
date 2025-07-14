#include "World.h"

#include <ctime>
#include <memory_resource>
#include <random>

#include "Chunk.h"
#include "renderer/ShaderLibrary.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Registry.h"

namespace Terra {
    World::World(): tileAtlas(RESOURCES_PATH "tileAtlas.png") {
        srand(time(NULL));
        seed = rand();
        Renderer::initialize();
        Registry::registerBaseItems();
        defaultShader = ShaderLibrary::get("default").get();
        loadedChunks.resize(16);
    }

    World::~World() {

    }

    void World::generateChunks() {
        int index = 0;
        for (int x = -2; x < 2; x++) {
            for (int y = -2; y < 2; y++) {
                loadedChunks[index++] = generateChunk(glm::vec2(x, y));
            }
        }
    }

    Chunk World::generateChunk(glm::vec2 chunkPos) {
        Tile chunkArray[16][16] {};
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                chunkArray[x][y] = Tile(0);
            }
        }

        chunkArray[8][8] = Tile(1);

        return Chunk {
            chunkPos,
            chunkArray
        };
    }

    constexpr int CHUNK_WIDTH = 16;
    constexpr int CHUNK_HEIGHT = 16;
    constexpr int TILE_WIDTH = 16;
    constexpr int TILE_HEIGHT = 16;

    void World::render() {
        float halfW = 1920 / (4.0f);
        float halfH = 1080 / (4.0f);
        defaultShader->activate();
        tileAtlas.bind();

        for (Chunk& chunk: loadedChunks) {
            for (int x = 0; x < CHUNK_WIDTH; ++x) {
                for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                    glm::vec2 tilePos = glm::vec2(
                        (chunk.chunkPos.x * CHUNK_WIDTH + x) * TILE_WIDTH - CHUNK_WIDTH * CHUNK_HEIGHT / 2,
                        (chunk.chunkPos.y * CHUNK_HEIGHT + y) * TILE_HEIGHT - CHUNK_WIDTH * CHUNK_HEIGHT / 2
                    );
                    glm::mat4 mvp = glm::ortho(-halfW, halfW, -halfH, halfH, -1.0f, 1.0f)
                                    * glm::translate(glm::mat4(1.0f), glm::vec3(tilePos, 0.0f));
                    defaultShader->setMatrix4x4("mvp", value_ptr(mvp));

                    defaultShader->setInt("tileID", chunk.tiles[x][y].getTileData()->startFrame);

                    Renderer::renderQuad(16, 16);
                }
            }
        }
    }
}
