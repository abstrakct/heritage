/*
 * Sound stuff
 *
 * Copyright (C) 2013
 *
 */

#ifndef _SOUND_H
#define _SOUND_H

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#define SOUND_UNKNOWN
#define SOUND_MUSIC_HOUSE_OF_LEAVES  1
#define SOUND_EFFECT_STORM01  2
#define SOUND_EFFECT_RAIN01   3


enum soundtype {
    sound_none = -1,
    music = 0,
    effect = 1
};

struct sound_def {
    int num;
    soundtype type;
    char filename[256];
};

#define NUM_SOUNDS 3

class SoundBit {
    public:
        SoundBit();
        SoundBit(char *name, soundtype t, int i);
        ~SoundBit();

        int id;
        soundtype type;
        char filename[256];
};

class SoundEngine {
    public:
        SoundEngine();
        ~SoundEngine();
        int initialize();
        void load_file(const char *filename);
        void load_all_files();
        void load_sound_def(int i);
        void play_sound(int sound);
        void play_sound(int sound, int ms);       // play sound, fade in ms milliseconds.
        void play_sound_infinite_loop(int sound);
        void play_music(int sound);
        void set_sound_volume(int sound, int volume);
        void pause_sound(int sound);
        void pause_sound(int sound, int ms);
    private:
        struct {
            Mix_Chunk *chunk;
            Mix_Music *music;
            int channel;
        } s[NUM_SOUNDS];
        int sounds_count;
        int audio_rate;
        Uint16 audio_format;
        int audio_channels;
        int audio_buffers;
};


#endif
// vim: fdm=syntax guifont=Terminus\ 8

