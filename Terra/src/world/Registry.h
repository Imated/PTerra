#pragma once
#include <map>

#include "Tile.h"
#include <cstdint>
#include <string>
#include <unordered_map>

namespace Terra {
    struct TileData;

    class Registry {
        public:
        static void registerBaseItems();
        static void registerTile(const TileData &tile);
        static TileData* getTile(uint8_t id);
        static uint8_t getRuleTileId(uint8_t id, uint8_t hgfedcba);
        private:
        static void setMap(uint8_t id, const std::string &patternStr, int frameIndex);
        static std::pmr::map<uint8_t, TileData> tileRegistry;
        static std::unordered_map<uint8_t, std::unordered_map<uint8_t, uint8_t>> ruleRegistry;
    };
}

