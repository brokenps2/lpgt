#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>
#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>
#include "Audio.h"
#include "Files.h"

ALCcontext* audioContext;
ALCdevice* audioDevice;

void gtmaInitAudio() {
    const ALCchar* defaultDevice = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    audioDevice = alcOpenDevice(defaultDevice);
    if (!audioDevice) {
        printf("unable to open default audio device\n");
        exit(1);
    }

    audioContext = alcCreateContext(audioDevice, NULL);
    if (!audioContext) {
        printf("unable to create audio context\n");
        alcCloseDevice(audioDevice);
        exit(1);
    }

    if (!alcMakeContextCurrent(audioContext)) {
        printf("unable to make AL context current\n");
        alcDestroyContext(audioContext);
        alcCloseDevice(audioDevice);
        exit(1);
    }
}

void gtmaCreateTrack(Track* track, const char* path, bool loop, float vol) {
    drwav_int16* rawAudioBuffer;
    unsigned int channels, sampleRate;
    drwav_uint64 sampleCount;
    rawAudioBuffer = drwav_open_file_and_read_pcm_frames_s16(res(path), &channels, &sampleRate, &sampleCount, NULL);

    if(!rawAudioBuffer) {
        printf("unable to open audio file\n");
        free(rawAudioBuffer);
        exit(1);
    }

    int format = -1;
    if (channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else {
        printf("unsupported number of channels: %d\n", channels);
        free(rawAudioBuffer);
        exit(1);
    }

    alGenBuffers(1, &track->bufferID);
    alBufferData(track->bufferID, format, rawAudioBuffer, sampleCount * channels * sizeof(drwav_int16), sampleRate);

    alGenSources(1, &track->sourceID);
    alSourcei(track->sourceID, AL_BUFFER, track->bufferID);
    if(loop) {
        alSourcei(track->sourceID, AL_LOOPING, AL_TRUE);
    } else {
        alSourcei(track->sourceID, AL_LOOPING, AL_FALSE);
    }
    alSourcei(track->sourceID, AL_POSITION, 0);
    alSourcei(track->sourceID, AL_GAIN, vol);

    free(rawAudioBuffer);

}

void gtmaCreateSound(Sound* sound, const char* path, bool loop, float vol, vec3 position) {
    drwav_int16* rawAudioBuffer;
    unsigned int channels, sampleRate;
    drwav_uint64 sampleCount;
    rawAudioBuffer = drwav_open_file_and_read_pcm_frames_s16(res(path), &channels, &sampleRate, &sampleCount, NULL);

    if(!rawAudioBuffer) {
        printf("unable to open audio file\n");
        free(rawAudioBuffer);
        exit(1);
    }

    int format = -1;
    if (channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else {
        printf("unsupported number of channels: %d\n", channels);
        free(rawAudioBuffer);
        exit(1);
    }

    alGenBuffers(1, &sound->bufferID);
    alBufferData(sound->bufferID, format, rawAudioBuffer, sampleCount * channels * sizeof(drwav_int16), sampleRate);

    alGenSources(1, &sound->sourceID);
    alSourcei(sound->sourceID, AL_BUFFER, sound->bufferID);
    if(loop) {
        alSourcei(sound->sourceID, AL_LOOPING, AL_TRUE);
    } else {
        alSourcei(sound->sourceID, AL_LOOPING, AL_FALSE);
    }
    alSourcei(sound->sourceID, AL_GAIN, vol);

    alSource3f(sound->sourceID, AL_POSITION, position[0], position[1], position[2]);

    alDistanceModel(AL_LINEAR_DISTANCE);

    alSourcef(sound->sourceID, AL_REFERENCE_DISTANCE, 1.0f);
    alSourcef(sound->sourceID, AL_MAX_DISTANCE, 15 * vol);
    alSourcef(sound->sourceID, AL_ROLLOFF_FACTOR, 1.0f);

    free(rawAudioBuffer);

}

void gtmaSetSoundPosition(Sound* sound, vec3 position) {
    alSource3f(sound->sourceID, AL_POSITION, position[0], position[1], position[2]);
}

void gtmaUpdateAudio(vec3 camPos, vec3 cameraDir) {
    float orient[6];
    orient[0] = cameraDir[0];
    orient[1] = cameraDir[1];
    orient[2] = cameraDir[2];
    orient[3] = 0;
    orient[4] = 1;
    orient[5] = 0;
    alListener3f(AL_POSITION, camPos[0], camPos[1], camPos[2]);
    alListenerfv(AL_ORIENTATION, orient);
}

void gtmaDisposeTrack(Track* track) {
    alDeleteSources(1, &track->sourceID);
    alDeleteBuffers(1, &track->bufferID);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);
}

void gtmaDeleteSound(Sound* sound) {
    alDeleteSources(1, &sound->sourceID);
    alDeleteBuffers(1, &sound->bufferID);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);
}

void gtmaPlaySound(Sound* sound) {
    int state;
    alGetSourcei(sound->sourceID, AL_SOURCE_STATE, &state);
    if(state == AL_STOPPED) {
        sound->isPlaying = false;
        alSourcei(sound->sourceID, AL_POSITION, 0);
    }

    if(!sound->isPlaying) {
        alSourcePlay(sound->sourceID);
        sound->isPlaying = true;
    }
}

void gtmaPlaySoundFrom(Sound* sound, int seconds) {
    alSourcei(sound->sourceID, AL_SEC_OFFSET, seconds);
    int state;
    alGetSourcei(sound->sourceID, AL_SOURCE_STATE, &state);
    if(state == AL_STOPPED) {
        sound->isPlaying = false;
        alSourcei(sound->sourceID, AL_POSITION, 0);
    }

    if(!sound->isPlaying) {
        alSourcePlay(sound->sourceID);
        sound->isPlaying = true;
    }
}

void gtmaPlayTrack(Track* track) {
    int state;
    alGetSourcei(track->sourceID, AL_SOURCE_STATE, &state);
    if(state == AL_STOPPED) {
        track->isPlaying = false;
        alSourcei(track->sourceID, AL_POSITION, 0);
    }

    if(!track->isPlaying) {
        alSourcePlay(track->sourceID);
        track->isPlaying = true;
    }
}

void gtmaPlayTrackFrom(Track* track, int seconds) {
    alSourcei(track->sourceID, AL_SEC_OFFSET, seconds);
    int state;
    alGetSourcei(track->sourceID, AL_SOURCE_STATE, &state);
    if(state == AL_STOPPED) {
        track->isPlaying = false;
        alSourcei(track->sourceID, AL_POSITION, 0);
    }

    if(!track->isPlaying) {
        alSourcePlay(track->sourceID);
        track->isPlaying = true;
    }
}

void gtmaStopSound(Sound* sound) {
    alSourceStop(sound->sourceID);
    alSourcei(sound->sourceID, AL_SEC_OFFSET, 0);
    sound->isPlaying = false;
}

void gtmaStopTrack(Track* track) {
    alSourceStop(track->sourceID);
    alSourcei(track->sourceID, AL_SEC_OFFSET, 0);
    track->isPlaying = false;
}

void gtmaCloseAudio() {
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);
}
