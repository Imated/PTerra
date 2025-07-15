#include "Random.h"

#include <chrono>

namespace Terra {
    void Random::init() {
        rngDevice = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    }

    void Random::setSeed(uint32_t inSeed) {
       seed = inSeed;
    }
}
