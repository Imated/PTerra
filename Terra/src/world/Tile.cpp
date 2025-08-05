#include "Tile.h"

#include "World.h"

namespace Terra {
    Tile::Tile(uint8_t id, glm::ivec2 pos, bool isGroundTile) {
        this->id = id;
        this->pos = pos;
        this->isGround = isGroundTile;
        Tile::update();
    }

    TileData* Tile::getTileData() {
        return Registry::getTile(id);
    }

    void Tile::onPlace() {
        for (int y = -1; y < 2; y++) {
            for (int x = -1; x < 2; x++) {
                World::getGlobalTileAt(pos + glm::ivec2(x, y))->update();
            }
        }
    }

    void Tile::update() {
        currentFrame = getTileData()->stateLocations[0];
    }

    uint8_t Tile::getId() {
        return id;
    }

    uint8_t Tile::getFrame() {
        return currentFrame;
    }
}
