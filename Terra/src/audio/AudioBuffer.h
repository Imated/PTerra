#pragma once
#include <vector>

#include "OpenAL/include/AL/al.h"

namespace Terra {
    struct StreamData;
}

namespace Terra {
    class AudioBuffer {
    public:
        AudioBuffer();
        ~AudioBuffer();
        StreamData createLongBufferSound(const char* filename);
        ALuint createSound(const char* filename);
        bool removeSound(const ALuint& buffer);
    private:
        std::vector<ALuint> soundEffectBuffers;
    };
}
