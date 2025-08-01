#pragma once
#include <memory>
#include <unordered_map>

#include "AudioBuffer.h"
#include "AudioSource.h"
#include "OpenAL/include/AL/alc.h"

namespace Terra {
    class Audio {
    public:
        ~Audio();
        static void init();
        static void update();

        static void loadSound(std::string id, const char* filename, bool streamed);
        static void unloadSound(std::string id);
        static ALuint getSound(std::string id);
        static AudioSource* playSound(std::string soundID, float pitch = 1.f, float gain = 1.f, glm::vec2 position = {}, glm::vec2 velocity = {}, bool loop = false);
    private:
        static int getFreeAudioSource();

        static inline ALCdevice* ALCdevice;
        static inline ALCcontext* ALCcontext;
        static inline std::unique_ptr<AudioBuffer> buffer;
        static inline std::vector<std::unique_ptr<AudioSource>> sourcesPool;
        static inline std::unordered_map<std::string, ALuint> sourcesMap;
        static inline std::unordered_map<std::string, StreamData> streamMap;
    };
}
