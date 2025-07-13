#pragma once
#include <vector>

#include "glm/vec2.hpp"

namespace Terra {
    struct Tile {
        char tileState;
        std::vector<glm::vec2> frameLocations;
        std::vector<glm::vec2> stateLocations;
    };
}
