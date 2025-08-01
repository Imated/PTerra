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

    bool AudioSource::streamBuffer(ALuint buffer) {
        short samples[BUFFER_SAMPLES * 2]; // stereo

        sf_count_t frames = sf_readf_short(stream.file, samples, BUFFER_SAMPLES);
        if (frames == 0) {
            stream.isFinished = true;
            return false;
        }

        alBufferData(buffer, stream.format, samples,
                     frames * stream.info.channels * sizeof(short),
                     stream.info.samplerate);
        return true;
    }

    void AudioSource::startStreaming(const StreamData &data) {
        stop();

        stream = data;
        isStreaming = true;
        stream.isFinished = false;

        alGenBuffers(NUM_AUDIO_BUFFERS, streamBuffers);

        for (int i = 0; i < NUM_AUDIO_BUFFERS; ++i) {
            if (!streamBuffer(streamBuffers[i]))
                break;
        }

        alSourceQueueBuffers(source, NUM_AUDIO_BUFFERS, streamBuffers);
        alSourcePlay(source);
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

    void AudioSource::updateStream() {
        if (!isStreaming)
            return;

        int processed = 0;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

        while (processed-- > 0) {
            ALuint buffer;
            alSourceUnqueueBuffers(source, 1, &buffer);

            if (streamBuffer(buffer))
                alSourceQueueBuffers(source, 1, &buffer);
        }

        ALint state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING && !stream.isFinished) {
            alSourcei(source, AL_BUFFER, static_cast<ALint>(buffer));
            alSourcef(source, AL_PITCH, pitch);
            alSourcef(source, AL_GAIN, gain);
            alSource3f(source, AL_POSITION, position.x, 0, position.y);
            alSource3f(source, AL_VELOCITY, velocity.x, 0, velocity.y);
            alSourcei(source, AL_LOOPING, loop);
            alSourcePlay(source);
        }
    }

    ALuint AudioSource::getSource() {
        return source;
    }

    bool AudioSource::isPlaying() const {
        ALint state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

    bool AudioSource::streaming() const {
        return isStreaming;
    }

    void AudioSource::stop() {
        alSourceStop(source);
        alSourcei(source, AL_BUFFER, 0);
        if (isStreaming) {
            alDeleteBuffers(NUM_AUDIO_BUFFERS, streamBuffers);
            sf_close(stream.file);
            isStreaming = false;
        }
    }

    void AudioSource::pause() const {
        alSourcePause(source);
    }

    void AudioSource::resume() const {
        alSourcePlay(source);
    }
}
