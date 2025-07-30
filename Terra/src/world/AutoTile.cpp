#include "AutoTile.h"
#include "misc/Logger.h"
#include "Chunk.h"
#include "World.h"

namespace Terra {
    AutoTile::AutoTile(uint8_t id, glm::ivec2 pos) {
        this->id = id;
        this->pos = pos;
    }

    void AutoTile::onPlace() {
        update();
    }

    void AutoTile::update() {
        int i = 7;
        hgfedcba = 0;
        bool foundNullTile = false;
        // loop through all neighbors and check if it has the same id as the current tile, iif it does then do some maths idk how the maths work u wrote it
        for (int x = -1; x < 2; x++) {
            for (int y = -1; y < 2; y++) {
                if (glm::vec2(x, y) != glm::vec2(0)) {
                    glm::ivec2 neighbor = pos + glm::ivec2(x, y);
                    const auto tile = World::getGlobalTileAt(neighbor);
                    if (tile) {
                        if (tile->getId() == id) {
                            hgfedcba |= (1 << i);
                        }
                    }
                    else {
                        DEBUG("got nullptr as tile: %i %i; %i %i. for position %i %i", pos.x, pos.y, x, y, neighbor.x, neighbor.y);
                        foundNullTile = true;
                    }
                    i--;
                }
            }
        }
        currentFrame = foundNullTile ? 1 : getTileData()->stateLocations[Registry::getRuleTileId(hgfedcba)];
    }
}
