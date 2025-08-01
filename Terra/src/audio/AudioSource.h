#pragma once

#include "sndfile.h"
#include "glm/vec2.hpp"
#include "misc/Constants.h"
#include "OpenAL/include/AL/al.h"

namespace Terra {
    struct StreamData {
        SNDFILE* file;
        SF_INFO info;
        ALenum format;
        bool isFinished;
    };

    class AudioSource {
    public:
        AudioSource();
        ~AudioSource();

        bool streamBuffer(ALuint buffer);

        void startStreaming(const StreamData& data, float pitch = 1.f, float gain = 1.f, glm::vec2 position = glm::vec2(0.f, 0.f), glm::vec2 velocity = glm::vec2(0.f, 0.f), bool loop = false);
        void play(ALuint bufferToPlayer, float pitch = 1.f, float gain = 1.f, glm::vec2 position = glm::vec2(0.f, 0.f), glm::vec2 velocity = glm::vec2(0.f, 0.f), bool loop = false);

        void updateStream();

        ALuint getSource();
        bool isPlaying() const;
        bool streaming() const;

        void stop();
        void pause() const;
        void resume() const;

        void setPitch(float pitch) { this->pitch = pitch; }
        void setGain(float gain) { this->gain = gain; }
        void setPosition(glm::vec2 position) { this->position = position; }
        void setVelocity(glm::vec2 velocity) { this->velocity = velocity; }
        void setLoop(bool loop) { this->loop = loop; }

        void getPitch(float& pitch) const { pitch = this->pitch; }
        void getGain(float& gain) const { gain = this->gain; }
        void getPosition(glm::vec2& position) const { position = this->position; }
        void getVelocity(glm::vec2& velocity) const { velocity = this->velocity; }
        bool getLoop() const { return this->loop; }
    private:
        ALuint source;
        float pitch = 1.f;
        float gain = 1.f;
        glm::vec2 position = glm::vec2(0.f, 0.f);
        glm::vec2 velocity = glm::vec2(0.f, 0.f);
        bool loop = false;
        ALuint buffer = 0;
        ALuint streamBuffers[NUM_AUDIO_BUFFERS];
        StreamData stream;
        bool isStreaming = false;
    };
}
