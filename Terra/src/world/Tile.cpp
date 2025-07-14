#include "Tile.h"

namespace Terra {
    Tile::Tile(uint8_t id) {
        this->id = id;
    }

    TileData* Tile::getTileData() {
        return Registry::getTile(id);
    }
}
