#include "Tile.h"

namespace Terra {
    Tile::Tile(uint8_t id, glm::ivec2 pos) {
        this->id = id;
        this->pos = pos;
    }

    TileData* Tile::getTileData() {
        return Registry::getTile(id);
    }

    void Tile::onPlace() {
        update();
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
