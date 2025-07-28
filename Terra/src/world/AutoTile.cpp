#include "AutoTile.h"
#include "misc/Logger.h"
#include "Chunk.h"
#include "WorldHelper.h"

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
        for (int x = -1; x < 2; x++) {
            for (int y = -1; y < 2; y++) {
                if (glm::vec2(x, y) != glm::vec2(0)) {
                    if (const auto tile = WorldHelper::getTileAt(pos + glm::ivec2(x, y)); tile != nullptr) {
                        if (tile->getId() == id) {
                            hgfedcba |= (1 << i);
                        }
                    }
                    else
                        ERR("eeeeeee");
                    i--;
                }
            }
        }
        currentFrame = getTileData()->stateLocations[Registry::getRuleTileId(hgfedcba)];
    }
}
