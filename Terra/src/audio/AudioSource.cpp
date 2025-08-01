#include "AudioSource.h"

#include "misc/Logger.h"

namespace Terra {
    AudioSource::AudioSource() {
        alGenSources(1, &source);
        alSourcef(source, AL_PITCH, pitch);
        alSourcef(source, AL_GAIN, gain);
        alSource3f(source, AL_POSITION, position.x, 0, position.y);
        alSource3f(source, AL_VELOCITY, velocity.x, 0, velocity.y);
        alSourcei(source, AL_LOOPING, loop);
    }

    AudioSource::~AudioSource() {
        alDeleteSources(1, &source);
    }

    void AudioSource::play(const ALuint bufferToPlayer, float pitch, float gain, glm::vec2 position, glm::vec2 velocity, bool loop) {

        this->pitch = pitch;
        this->gain = gain;
        this->position = position;
        this->velocity = velocity;
        this->loop = loop;

        if (bufferToPlayer != buffer) {
            buffer = bufferToPlayer;
        }

        alSourceStop(source); // stop in case it was playing
        alSourcei(source, AL_BUFFER, static_cast<ALint>(buffer));
        alSourcef(source, AL_PITCH, pitch);
        alSourcef(source, AL_GAIN, gain);
        alSource3f(source, AL_POSITION, position.x, 0, position.y);
        alSource3f(source, AL_VELOCITY, velocity.x, 0, velocity.y);
        alSourcei(source, AL_LOOPING, loop);

        alSourcePlay(source);
    }

    ALuint AudioSource::getSource() {
        return source;
    }

    bool AudioSource::isPlaying() const {
        ALint state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

    void AudioSource::stop() const {
        alSourceStop(source);
    }

    void AudioSource::pause() const {
        alSourcePause(source);
    }

    void AudioSource::resume() const {
        alSourcePlay(source);
    }
}
