#pragma once
#include <cstdint>
#include <random>

namespace Terra {
    class Random {
    public:
        static void init();
        static void setSeed(uint32_t seed);

        template <typename T>
        static T get(T min, T max) {
            if constexpr (std::is_integral_v<T>)
                return std::uniform_int_distribution<T>(min, max)(rngDevice);
            else
                return std::uniform_real_distribution<T>(min, max)(rngDevice);
        }

    private:
        static inline uint32_t seed = 0;
        static inline std::mt19937 rngDevice;
    };
}
