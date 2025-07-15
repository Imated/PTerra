#include "Registry.h"

namespace Terra {
    std::pmr::map<uint8_t, TileData> Registry::tileRegistry;

    void Registry::registerBaseItems() {
        registerTile(TileData
            {
                0,
                1,
                std::vector<uint8_t> {0}
            });
        registerTile(TileData
            {
                106,
                1,
                std::vector<uint8_t> {0}
            });
        registerTile(TileData
    {
        126,
        1,
        std::vector<uint8_t> {0}
    });
        registerTile(TileData
    {
        128,
        1,
        std::vector<uint8_t> {0}
    });
        registerTile(TileData
    {
        127,
        1,
        std::vector<uint8_t> {0}
    });
        registerTile(TileData
    {
        105,
        1,
        std::vector<uint8_t> {0}
    });
        registerTile(TileData
    {
        107,
        1,
        std::vector<uint8_t> {0}
    });
        registerTile(TileData
    {
        85,
        1,
        std::vector<uint8_t> {0}
    });
        registerTile(TileData
    {
        86,
        1,
        std::vector<uint8_t> {0}
    });
        registerTile(TileData
    {
        84,
        1,
        std::vector<uint8_t> {0}
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
