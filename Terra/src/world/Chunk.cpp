#include "Chunk.h"

#include "AutoTile.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"
#include "misc/Constants.h"
#include "renderer/Renderer.h"

namespace Terra {

    Chunk::Chunk(glm::ivec2 chunkPos, std::array<std::array<std::unique_ptr<Tile>, 16>, 16> tilesIn) : chunkPos(chunkPos), tiles(std::move(tilesIn)) { }

    Chunk::~Chunk() {

    }

    void Chunk::render(glm::mat4 vp, Shader* shader) {
        std::vector<uint32_t> tileFrames;

        //TODO: Figure out how and WHY this code segfaults

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
        //shader->setInt("tileID", tiles[0][0].getTileData()->startFrame);

        Renderer::renderQuad();
    }

    Tile* Chunk::getTileAt(glm::ivec2 pos) const {
        if (pos.x < 0 || pos.y < 0 || pos.x >= 16 || pos.y >= 16)
            return tiles[0][0].get();
        return tiles[pos.x][pos.y].get();
    }
}
