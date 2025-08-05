#include "Registry.h"

#include <stdexcept>

#include "misc/Logger.h"

namespace Terra {
    std::pmr::map<uint8_t, TileData> Registry::tileRegistry;
    std::unordered_map<uint8_t, std::unordered_map<uint8_t, uint8_t>> Registry::ruleRegistry;

    void Registry::registerBaseItems() {
        INFO("Registering Base Items");
        registerTile(TileData
            {
                1,
                std::vector<uint8_t> {0}
            }
        );
        registerTile(TileData
            {
                1,
                std::vector<uint8_t> {1}
            });
        registerTile(TileData
            {
                1,
                std::vector<uint8_t> {126, 127, 128, 105, 106, 107, 84, 85, 86, 129, 108, 87, 63, 64, 65, 66, 130, 131, 109, 110}
            });
        setMap(2, "X1X01X0X", 0);
        setMap(2, "X1X11X0X", 1);
        setMap(2, "X1X10X0X", 2);
        setMap(2, "X1X01X1X", 3);
        setMap(2, "X1X11X1X", 4);
        setMap(2, "X1X10X1X", 5);
        setMap(2, "X0X01X1X", 6);
        setMap(2, "X0X11X1X", 7);
        setMap(2, "X0X10X1X", 8);
        setMap(2, "X1X00X0X", 9);
        setMap(2, "X1X00X1X", 10);
        setMap(2, "X0X00X1X", 11);
        setMap(2, "X0X01X0X", 12);
        setMap(2, "X0X11X0X", 13);
        setMap(2, "X0X10X0X", 14);
        setMap(2, "X0X00X0X", 15);
        setMap(2, "11011111", 16);
        setMap(2, "01111111", 17);
        setMap(2, "11111110", 18);
        setMap(2, "11111011", 19);
        registerTile(TileData
            {
                1,
                std::vector<uint8_t> {20}
            });
    }

    void Registry::registerTile(const TileData& tile) {
        auto newID = static_cast<uint8_t>(tileRegistry.size());
        tileRegistry[newID] = tile;
    }

    TileData* Registry::getTile(const uint8_t id) {
        return &tileRegistry.at(id);
    }

    uint8_t Registry::getRuleTileId(uint8_t id, uint8_t hgfedcba) {
        try {
            return ruleRegistry.at(id).at(hgfedcba);

        } catch (const std::out_of_range& e) {
            WARN("Could not find tilemap for sequence %i", hgfedcba);
            //ASSERT("Could not find matching 'hgfedcba' in map: %i", hgfedcba);
        }
        return 146;
    }

    void Registry::setMap(uint8_t id, const std::string& patternStr, int frameIndex) {
        if (patternStr.length() != 8) {
            ERR("Invalid pattern length: %s", patternStr.c_str());
            return;
        }

        uint8_t fixedBits = 0;
        uint8_t wildcardMask = 0;

        for (int i = 0; i < 8; ++i) {
            char c = patternStr[i];
            int bitPos = 7 - i;

            if (c == '0') {
                // do nothing
            } else if (c == '1') {
                fixedBits |= (1 << bitPos);
            } else if (c == 'X' || c == 'x') {
                wildcardMask |= (1 << bitPos);
            } else {
                ERR("Invalid character in rule pattern: %c", c);
                return;
            }
        }

        if (wildcardMask == 0) {
            ruleRegistry[id][fixedBits] = frameIndex;
            return;
        }

        for (uint16_t combo = 0; combo < 256; ++combo) {
            if ((combo & ~wildcardMask) == (fixedBits & ~wildcardMask)) {
                ruleRegistry[id][combo] = frameIndex;;
            }
        }
    }
}
