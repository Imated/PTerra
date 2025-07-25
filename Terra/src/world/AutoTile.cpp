#include "AutoTile.h"

#include "World.h"

namespace Terra {
    AutoTile::AutoTile(uint8_t id, glm::ivec2 pos) {
        this->id = id;
        this->pos = pos;
    }

    void AutoTile::onPlace() {
         int i = 7;
         for (int x = -1; x < 2; x++) {
             for (int y = -1; y < 2; y++) {
                 if (glm::vec2(x, y) != glm::vec2(0)) {
                    //getTile(pos + glm::vec2(x, y).update();
                    if (const auto tile = World::getTileAt(pos + glm::ivec2(x, y)); tile != nullptr)
                        if (tile->getId() == id)
                            hgfedcba |= (1 << i);
                     i--;
                 }
             }
         }
        //DEBUG("%i", hgfedcba);
    }

    void AutoTile::update() {

    }
}
