#include "Chunk.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"
#include "misc/Constants.h"
#include "renderer/Renderer.h"

namespace Terra {
    void Chunk::render(glm::mat4 vp, Shader* shader) {
        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                glm::vec2 offset = glm::vec2(
                    -MAX_CHUNKS_X * CHUNK_WIDTH * TILE_WIDTH / 2.0f,
                    -MAX_CHUNKS_Y * CHUNK_HEIGHT * TILE_HEIGHT / 2.0f
                );

                glm::vec2 tilePos = glm::vec2(
                    (chunkPos.x * CHUNK_WIDTH + x) * TILE_WIDTH,
                    (chunkPos.y * CHUNK_HEIGHT + y) * TILE_HEIGHT
                ) + offset;

                auto mvp = glm::translate(vp, glm::vec3(tilePos, 0.0f));
                shader->setMatrix4x4("mvp", value_ptr(mvp));
                shader->setInt("tileID", tiles[x][y].getTileData()->startFrame);
                Renderer::renderQuad(TILE_WIDTH, TILE_HEIGHT);
            }
        }
    }
}
