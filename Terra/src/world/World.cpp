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

namespace Terra {



    World::World(): tileAtlas(RESOURCES_PATH "tileAtlas.png") {
        seed = Random::get<int32_t>(INT32_MIN, INT32_MAX);
        DEBUG("Random seed: %i", seed);
        loadedChunks.resize(MAX_CHUNKS_X * MAX_CHUNKS_Y);
    }

    World::~World() = default;

    void World::init() {
        auto shaderPtr = ShaderLibrary::get("default");
        if (!shaderPtr) {
            ERR("Shader 'default' not loaded!");
            return;
        }

        defaultShader = shaderPtr.get();
        defaultShader->activate();
        tileAtlas.bind();
    }

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
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                if (x == 0 && y == 15)
                    chunkArray[x][y] = Tile(2); // top-left
                else if (x == 15 && y == 15)
                    chunkArray[x][y] = Tile(3); // top-right
                else if (x == 0 && y == 0)
                    chunkArray[x][y] = Tile(9); // bottom-left
                else if (x == 15 && y == 0)
                    chunkArray[x][y] = Tile(8); // bottom-right
                else if (y == 15)
                    chunkArray[x][y] = Tile(4); // top edge
                else if (y == 0)
                    chunkArray[x][y] = Tile(7); // bottom edge
                else if (x == 0)
                    chunkArray[x][y] = Tile(5); // left edge
                else if (x == 15)
                    chunkArray[x][y] = Tile(6); // right edge
                else
                    chunkArray[x][y] = Tile(1); // center
            }
        }

        //chunkArray[8][8] = Tile(1);

        return Chunk {
            chunkPos,
            chunkArray
        };
    }

    #pragma optimize(...)
    void World::render() {
        glm::mat4 vp = Renderer::getCamera()->getViewMatrix() * Renderer::getCamera()->getProjectionMatrix();

        for (Chunk& chunk: loadedChunks) {
            chunk.render(vp, defaultShader);
        }
    }
}
