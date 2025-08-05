#include "Chunk.h"

#include "AutoTile.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"
#include "misc/Constants.h"
#include "renderer/Renderer.h"

namespace Terra {

    Chunk::Chunk(glm::ivec2 chunkPos,
        std::array<std::array<std::unique_ptr<Tile>, CHUNK_HEIGHT>, CHUNK_WIDTH> groundTilesIn
        ) : chunkPos(chunkPos), groundTiles(std::move(groundTilesIn)) {
        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                if (x == y)
                    topTiles[x][y] = std::make_unique<Tile>(1, glm::vec2(chunkPos.x + x, chunkPos.y + y));
                else
                    topTiles[x][y] = std::make_unique<Tile>(0, glm::vec2(chunkPos.x + x, chunkPos.y + y));
            }
        }
    }
    Chunk::Chunk(glm::ivec2 chunkPos,
        std::array<std::array<std::unique_ptr<Tile>, CHUNK_HEIGHT>, CHUNK_WIDTH> groundTilesIn,
        std::array<std::array<std::unique_ptr<Tile>, CHUNK_HEIGHT>, CHUNK_WIDTH> topTilesIn
    ) : chunkPos(chunkPos), groundTiles(std::move(groundTilesIn)), topTiles(std::move(topTilesIn)) { }

    Chunk::~Chunk() {

    }

    void Chunk::render(glm::mat4 vp, Shader* shader) {
        // INFO("chunk being rendered");
        // INFO("chunk at 0, 0 is %i", groundTiles[0][0]->getId());
        // INFO("chunk at 0, 0 is %i", topTiles[0][0]->getId());
        // throw std::runtime_error("Chunk::render() not implemented");

        glm::vec2 chunkPosTiles = glm::vec2(
             (chunkPos.x * CHUNK_WIDTH),
             (chunkPos.y * CHUNK_HEIGHT)
        );

        std::vector<uint32_t> groundFrames, topFrames;
        for (int y = 0; y < CHUNK_WIDTH; y++) {
            for (int x = 0; x < CHUNK_HEIGHT; x++) {
                groundFrames.push_back(groundTiles[x][y]->getFrame());
                topFrames.push_back(topTiles[x][y]->getFrame());
            }
        }
        auto mvp = glm::translate(vp, glm::vec3(chunkPosTiles, -LAYER_TILES_GROUND));
        mvp = glm::scale(mvp, glm::vec3(CHUNK_WIDTH, CHUNK_HEIGHT, 1.f));
        shader->setMatrix4x4("mvp", value_ptr(mvp));
        shader->setUIntArray("groundFrames", CHUNK_WIDTH*CHUNK_HEIGHT, groundFrames.data());
        shader->setUIntArray("topFrames", CHUNK_WIDTH*CHUNK_HEIGHT, topFrames.data());
        Renderer::renderQuad();
    }

    // gets tile at loacl tile position within chunk.
    Tile* Chunk::getTileAt(glm::ivec2 pos, bool top) const {
        if (pos.x < 0 || pos.y < 0 || pos.x >= CHUNK_WIDTH || pos.y >= CHUNK_HEIGHT)
            return nullptr;
        auto tile = top ? topTiles[pos.x][pos.y].get() : groundTiles[pos.x][pos.y].get();
        if (tile == nullptr) {
            ERR("Null?");
            return nullptr;
        }
        return tile;
    }
    void Chunk::setTileAt(glm::ivec2 pos, std::unique_ptr<Tile> tile, bool top) {
        if (pos.x < 0 || pos.y < 0 || pos.x >= CHUNK_WIDTH || pos.y >= CHUNK_HEIGHT)
            return;
        top ? topTiles[pos.x][pos.y] : groundTiles[pos.x][pos.y] = std::move(tile);
    }
}
