/*
 * Roguelike Engine in C++
 *
 * Sound specific stuff.
 *
 */

using namespace std;

#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#include "actor.h"
#include "player.h"
#include "common.h"
#include "sound.h"

struct sound_def sound_defs[NUM_SOUNDS] = {
        { 0, "test.wav"},
        { 1, "test.wav"},
};
        
SoundEngine::SoundEngine()
{
        audio_rate = 44100;
        audio_format = AUDIO_S16SYS;
        audio_channels = 2;
        audio_buffers = 4096;
        sounds_count = 0;
}

SoundEngine::~SoundEngine()
{
        Mix_CloseAudio();
}

void SoundEngine::load_file(const char *filename)
{
        s[sounds_count].chunk = Mix_LoadWAV(filename);
        if(s[sounds_count].chunk == NULL) {
                fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
        }

        sounds_count++;
}

void SoundEngine::load_all_files()
{
        int i;
        for(i = 0; i < NUM_SOUNDS; i++) {
                load_file(sound_defs[i].filename);
        }
}

void SoundEngine::play_sound(int sound)
{
        if(sound > sounds_count) {
                dbg("Tried to play unloaded sound!");
                return;
        }
        s[sound].channel = Mix_PlayChannel(-1, s[sound].chunk, 0);
        if(s[sound].channel == -1) {
                fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
        }

}

void SoundEngine::play_sound_infinite_loop(int sound)
{
        if(sound > sounds_count) {
                dbg("Tried to play unloaded sound!");
                return;
        }
        s[sound].channel = Mix_PlayChannel(-1, s[sound].chunk, -1);
        if(s[sound].channel == -1) {
                fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
        }
}

void SoundEngine::play_sound(int sound, int ms)
{
        if(sound > sounds_count) {
                dbg("Tried to play unloaded sound!");
                return;
        }
        s[sound].channel = Mix_FadeInChannel(-1, s[sound].chunk, 0, ms);
        if(s[sound].channel == -1) {
                fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
        }

}

void SoundEngine::pause_sound(int sound)
{
        Mix_Pause(s[sound].channel);
}

void SoundEngine::pause_sound(int sound, int ms)
{
        Mix_FadeOutChannel(s[sound].channel, ms);
}

void SoundEngine::set_sound_volume(int sound, int volume)
{
        Mix_VolumeChunk(s[sound].chunk, volume);
}

int SoundEngine::initialize()
{
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
                fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
                return 1;
        }

        if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
                exit(1);
        }

        return 0;
}
