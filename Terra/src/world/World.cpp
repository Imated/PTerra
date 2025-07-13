#include "World.h"

#include <ctime>
#include <random>

#include "Chunk.h"
#include "Chunk.h"
#include "Chunk.h"
#include "Chunk.h"

namespace Terra {
    World::World(): tileAtlas("resources/tileAtlas.png") {
        srand(time(NULL));
        seed = rand();
        renderer = Renderer();
        renderer.initialize();
    }

    World::~World() {

    }

    void World::generateChunks() {
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                loadedChunks[x][y] = generateChunk(glm::vec2(x, y));
            }
        }
    }

    Chunk World::generateChunk(glm::vec2 chunkPos) {
        char chunkArray[16][16];
        for (auto & x : chunkArray) {
            for (char & y : x) {
                y = 0;
                Renderer::renderQuad(16, 16);
            }
        }

        chunkArray[8][8] = 1;

        return Chunk {
            chunkPos,
            chunkArray
        };
    }
}
