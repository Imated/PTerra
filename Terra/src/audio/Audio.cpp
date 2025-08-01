#include "Audio.h"

#include "misc/Logger.h"

namespace Terra {
    void Audio::init() {
        ALCdevice = alcOpenDevice(nullptr);
        if (!ALCdevice) {
            ERR("Failed to get sound device! Imagine not having speakers thats just a skill issue icl :wilted_rose:");
        }

        ALCcontext = alcCreateContext(ALCdevice, nullptr);
        if (!ALCcontext) {
            ERR("Failed to create audio context! Another skill issue.");
        }
        if (!alcMakeContextCurrent(ALCcontext)) {
            ERR("Failed to make audio current. Sooo many skill issues");
        }

        const ALCchar* name = nullptr;
        if (alcIsExtensionPresent(ALCdevice, "ALC_ENUMERATION_EXT")) {
            name = alcGetString(ALCdevice, ALC_ALL_DEVICES_SPECIFIER);
        }
        if (!name || alcGetError(ALCdevice) != ALC_NO_ERROR) {
            name = alcGetString(ALCdevice, ALC_DEVICE_SPECIFIER);
        }
        INFO("Opened sound device: %s", name);

        buffer = std::make_unique<AudioBuffer>();
        sourcesPool.clear();
        sourcesPool.reserve(16);
        for (int i = 0; i < 16; ++i) {
            sourcesPool.push_back(std::make_unique<AudioSource>());
        }
    }

    void Audio::update() {
        for (auto& source: sourcesPool) {
            if (source->streaming())
                source->updateStream();
        }
    }

    void Audio::loadSound(std::string id, const char *filename, bool streamed) {
        if (!streamed)
            sourcesMap[id] = buffer->createSound(filename);
        else
            streamMap[id] = buffer->createLongBufferSound(filename);
    }

    void Audio::unloadSound(std::string id) {
        sourcesMap.erase(id);
    }

    ALuint Audio::getSound(std::string id) {
        auto buffer = sourcesMap.find(id);
        if (buffer == sourcesMap.end()) {
            ERR("No sound found for: %s", id.c_str());
            return AL_NONE;
        }
        return buffer->second;
    }

    AudioSource* Audio::playSound(std::string soundID, float pitch, float gain, glm::vec2 position, glm::vec2 velocity, bool loop) {
        const auto sourceIndex = getFreeAudioSource();
        if (sourceIndex == -1) {
            ERR("Failed to get free sound source!");
            return nullptr;
        }
        auto& source = sourcesPool[sourceIndex];
        if (streamMap.contains(soundID))
            source->startStreaming(streamMap[soundID], pitch, gain, position, velocity, loop);
        else
            source->play(sourcesMap[soundID], pitch, gain, position, velocity, loop);
        return source.get();
    }

    void Audio::playSound(std::string soundID, AudioSource &source, float pitch, float gain, glm::vec2 position,
        glm::vec2 velocity, bool loop) {
        if (streamMap.contains(soundID))
            source.startStreaming(streamMap[soundID], pitch, gain, position, velocity, loop);
        else
            source.play(sourcesMap[soundID], pitch, gain, position, velocity, loop);
    }

    int Audio::getFreeAudioSource() {
        auto it = std::ranges::find_if(sourcesPool, [](const std::unique_ptr<AudioSource>& source) {
            return !source->isPlaying();
        });

        if (it != sourcesPool.end())
            return static_cast<int>(std::distance(sourcesPool.begin(), it));

        return -1;
    }

    Audio::~Audio() {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(ALCcontext);
        alcCloseDevice(ALCdevice);
    }

}
