#pragma once
#include <vector>

#include "OpenAL/include/AL/al.h"

namespace Terra {
    class AudioBuffer {
    public:
        AudioBuffer();
        ~AudioBuffer();
        ALuint createSound(const char* filename);
        bool removeSound(const ALuint& buffer);
    private:
        std::vector<ALuint> soundEffectBuffers;
    };
}
