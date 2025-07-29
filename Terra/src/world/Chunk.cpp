#include "Chunk.h"

#include "AutoTile.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"
#include "misc/Constants.h"
#include "renderer/Renderer.h"

namespace Terra {

    Chunk::Chunk(glm::ivec2 chunkPos, std::array<std::array<std::unique_ptr<Tile>, CHUNK_HEIGHT>, CHUNK_WIDTH> tilesIn) : chunkPos(chunkPos), tiles(std::move(tilesIn)) { }

    Chunk::~Chunk() {

    }

    void Chunk::render(glm::mat4 vp, Shader* shader) {
        std::vector<uint32_t> tileFrames;

        for (int x = 0; x < CHUNK_WIDTH; x++) {
            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                tileFrames.push_back(tiles[x][y]->getFrame());
            }
        }

        glm::vec2 chunkPosTiles = glm::vec2(
             (chunkPos.x * CHUNK_WIDTH),
             (chunkPos.y * CHUNK_HEIGHT)
         );
        auto mvp = glm::translate(vp, glm::vec3(chunkPosTiles, -LAYER_BACKGROUND));
        mvp = glm::scale(mvp, glm::vec3(CHUNK_WIDTH, CHUNK_HEIGHT, 0.f));
        shader->setMatrix4x4("mvp", value_ptr(mvp));
        shader->setUIntArray("tileFrames", 256, tileFrames.data());

        Renderer::renderQuad();
    }

    Tile* Chunk::getTileAt(glm::ivec2 pos) const {
        if (pos.x < 0 || pos.y < 0 || pos.x >= CHUNK_WIDTH || pos.y >= CHUNK_HEIGHT)
            return nullptr;
        return tiles[pos.x][pos.y].get();
    }
}
