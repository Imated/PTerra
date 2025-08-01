#include "AudioBuffer.h"

#include <cinttypes>

#include "AudioSource.h"
#include "sndfile.h"
#include "misc/Logger.h"
#include "OpenAL/include/AL/alext.h"

namespace Terra {
    ALuint AudioBuffer::createSound(const char *filename) {
        ALenum error;
        ALenum format;
        ALuint buffer;
        SNDFILE* sndfile;
        SF_INFO sfinfo;
        short* memoryBuffer;
        sf_count_t numberOfFrames;
        ALsizei numberOfBytes;

        sndfile = sf_open(filename, SFM_READ, &sfinfo);
        if (!sndfile) {
            ERR("Could not open sound file %s: %s", filename, sf_strerror(sndfile));
            return 0;
        }
        if (sfinfo.frames < 1 || sfinfo.frames > static_cast<sf_count_t>((INT_MAX / sizeof(short))) / sfinfo.channels) {
            ERR("Bad sample count in %s (%" PRId64 ")", filename, sfinfo.frames);
            sf_close(sndfile);
            return 0;
        }

        format = AL_NONE;
        if (sfinfo.channels == 1)
            format = AL_FORMAT_MONO16;
        else if (sfinfo.channels == 2)
            format = AL_FORMAT_STEREO16;
        else if (sfinfo.channels == 3) {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT2D_16;
        }
        else if (sfinfo.channels == 4) {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT3D_16;
        }
        if (!format) {
            ERR("Could not get format from sound file %s", filename);
            sf_close(sndfile);
            return 0;
        }

        memoryBuffer = static_cast<short*>(malloc(static_cast<size_t>(sfinfo.frames) * sfinfo.channels * sizeof(short)));
        numberOfFrames = sf_readf_short(sndfile, memoryBuffer, sfinfo.frames);
        if (numberOfFrames < 1) {
            free(memoryBuffer);
            sf_close(sndfile);
            ERR("Failed to read samples in %s (%" PRId64 ")", filename, numberOfFrames);
            return 0;
        }
        numberOfBytes = static_cast<ALsizei>(numberOfFrames * sfinfo.channels) * static_cast<ALsizei>(sizeof(short));

        buffer = 0;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, memoryBuffer, numberOfBytes, sfinfo.samplerate);

        free(memoryBuffer);
        sf_close(sndfile);

        error = alGetError();
        if (error != AL_NO_ERROR) {
            ERR("OpenAL Error: %s", alGetString(error));
            if (buffer && alIsBuffer(buffer))
                alDeleteBuffers(1, &buffer);
            return 0;
        }

        soundEffectBuffers.push_back(buffer);

        return buffer;
    }

    bool AudioBuffer::removeSound(const ALuint &buffer) {
        auto it = soundEffectBuffers.begin();
        while (it != soundEffectBuffers.end()) {
            if (*it == buffer) {
                alDeleteBuffers(1, &buffer);

                it = soundEffectBuffers.erase(it);

                return true;
            }
            else {
                ++it;
            }
        }
        return false;
    }

    AudioBuffer::AudioBuffer() {
        soundEffectBuffers.clear();
    }

    AudioBuffer::~AudioBuffer() {
        alDeleteBuffers(soundEffectBuffers.size(), soundEffectBuffers.data());
        soundEffectBuffers.clear();
    }

    StreamData AudioBuffer::createLongBufferSound(const char *filename) {
        StreamData data {};
        data.file = sf_open(filename, SFM_READ, &data.info);
        if (!data.file) {
            ERR("Failed to open stream file: %s", sf_strerror(nullptr));
            return data;
        }

        data.format = AL_NONE;
        if (data.info.channels == 1)
            data.format = AL_FORMAT_MONO16;
        else if (data.info.channels == 2)
            data.format = AL_FORMAT_STEREO16;
        else if (data.info.channels == 3) {
            if (sf_command(data.file, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
                data.format = AL_FORMAT_BFORMAT2D_16;
        }
        else if (data.info.channels == 4) {
            if (sf_command(data.file, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
                data.format = AL_FORMAT_BFORMAT3D_16;
        }
        if (!data.format) {
            ERR("Could not get format from sound file %s", filename);
            sf_close(data.file);
        }

        return data;
    }
}
