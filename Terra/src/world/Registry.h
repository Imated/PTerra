#pragma once
#include <map>

#include "Tile.h"
#include <cstdint>

namespace Terra {
    struct TileData;

    class Registry {
        public:
        static void registerBaseItems();
        static void registerTile(const TileData &tile);
        static TileData* getTile(uint8_t id);
        private:
        static std::pmr::map<uint8_t, TileData> tileRegistry;
    };
}

