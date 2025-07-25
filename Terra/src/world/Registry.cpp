#include "Registry.h"

namespace Terra {
    std::pmr::map<uint8_t, TileData> Registry::tileRegistry;

    void Registry::registerBaseItems() {
        registerTile(TileData
            {
                1,
                std::vector<uint8_t> {0}
            });
        registerTile(TileData
            {
                1,
                std::vector<uint8_t> {106}
            });
    }

    void Registry::registerTile(const TileData& tile) {
        auto newID = static_cast<uint8_t>(tileRegistry.size());
        tileRegistry[newID] = tile;
    }

    TileData* Registry::getTile(const uint8_t id) {
        return &tileRegistry[id];
    }
}
