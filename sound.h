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

struct sound_def {
       int num;
       char filename[256];
};

#define NUM_SOUNDS 2

class SoundEngine {
        public:
                SoundEngine();
                ~SoundEngine();
                int initialize();
                void load_file(const char *filename);
                void load_all_files();
                void play_sound(int sound);
                void play_sound(int sound, int ms);       // play sound, fade in ms milliseconds.
                void play_sound_infinite_loop(int sound);
                void set_sound_volume(int sound, int volume);
                void pause_sound(int sound);
                void pause_sound(int sound, int ms);
        private:
                struct {
                        Mix_Chunk *chunk;
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

