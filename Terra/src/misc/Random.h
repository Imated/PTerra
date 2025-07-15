#pragma once
#include <cstdint>
#include <random>

namespace Terra {
    class Random {
    public:
        static void init();
        static void setSeed(uint32_t seed);

        template <typename T>
        static T get(uint32_t min, uint32_t max) {
            return std::uniform_int_distribution<T>(min, max)(rngDevice);
        }

    private:
        static inline uint32_t seed = 0;
        static inline std::mt19937 rngDevice;
    };
}
