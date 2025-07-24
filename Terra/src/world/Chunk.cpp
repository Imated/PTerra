#include "Chunk.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"
#include "misc/Constants.h"
#include "renderer/Renderer.h"

namespace Terra {
    void Chunk::render(glm::mat4 vp, Shader* shader) {
        std::vector<glm::uint32_t> tileFrames;
        tileFrames.resize(CHUNK_WIDTH * CHUNK_HEIGHT);

        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                tileFrames[x + y * CHUNK_WIDTH] = tiles[x][y].getTileData()->startFrame;
            }
        }

        glm::vec2 tilePos = glm::vec2(
             (chunkPos.x * CHUNK_WIDTH),
             (chunkPos.y * CHUNK_HEIGHT)
         );

        auto mvp = glm::translate(vp, glm::vec3(tilePos, -LAYER_BACKGROUND));
        mvp = glm::scale(mvp, glm::vec3(CHUNK_WIDTH, CHUNK_HEIGHT, 0.f));
        shader->setMatrix4x4("mvp", value_ptr(mvp));
        shader->setUIntArray("tileFrames", 256, tileFrames.data());
        //shader->setInt("tileID", tiles[0][0].getTileData()->startFrame);

        Renderer::renderQuad();
    }
}
