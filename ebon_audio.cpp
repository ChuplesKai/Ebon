/**
 * Copyright 2014, Andrew Dobson
 *
 * @file ebon_audio.cpp
 * @author Andrew Dobson
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "ebon_audio.h"

/**
 *
 */
ebon_audio_t::ebon_audio_t()
{
    _bgm = _reserve_bgm =_music_a = _music_b = NULL;
    //Set the SFX to NULL
    for(unsigned i=0; i<MAX_MIX_CHUNKS; ++i)
        _sfx[i] = NULL;
    _play_music = false;
    _paused = false;
    _bgm_time = 0.0;
    _volume = MIX_MAX_VOLUME;
    _loops = -1;
    _fade_out = false;
    _fade_in = false;
    _fading = false;
    _have_reserve = false;
    _stored = false;
    _fade_in_time = _fade_out_time = 0;
    strcpy(_loaded_bgm,"");

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        std::cerr << "Warning: Audio could not open device!  Audio will not play.\n";
    }
}

/**
 *
 */
ebon_audio_t::~ebon_audio_t()
{
    //Free all the things
    stop_bgm();
    if(_bgm)
        Mix_FreeMusic(_bgm);
    if(_reserve_bgm)
        Mix_FreeMusic(_reserve_bgm);

    for(unsigned i=0; i<MAX_MIX_CHUNKS; ++i)
    {
        if(_sfx[i])
            Mix_FreeChunk((_sfx[i]));
    }

    //Shut down Mix
    Mix_CloseAudio();
    Mix_Quit();
}

/**
 *
 */
int ebon_audio_t::initialize()
{
    return Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
}

/**
 *
 */
void ebon_audio_t::frame()
{
    //Determine if the music is fading
    _fading = (Mix_FadingMusic() & (MIX_FADING_OUT | MIX_FADING_IN));

    //If we've received the signal to play music, play it
    if(_play_music)
    {
        if(Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(_bgm,_loops);
            Mix_VolumeMusic(_volume);
        }
        _play_music = false;
    }
    else if(_fade_out && !_fading) //Otherwise, see if we're fading music out
    {
        Mix_FadeOutMusic(_fade_out_time);
        _fade_out = false;
    }
    else if(_fade_in && !_fading) //Then, check if we're fading music in
    {
        //TODO: Handle the case that we've loaded a BGM before-hand
        Mix_FadeInMusic(_bgm, _loops, _fade_in_time);
        _fade_in = false;
    }

    //Now, we need to handle sfx
    for(unsigned i=0; i<MAX_MIX_CHUNKS; ++i)
    {
        //If there is a chunk there
        if(_sfx[i] != NULL)
        {
            //Check to see if we should play it
            if(_play_sfx[i])
            {
                printf("Start track %d\n",i);
                Mix_PlayChannel(i,_sfx[i],0);
                _play_sfx[i] = false;
            }
            else if(Mix_Playing(i) == 0) //Then check if it's done playing
            {
                printf("Stop track %d\n",i);
                Mix_FreeChunk(_sfx[i]);
                _sfx[i] = NULL;
            }
        }
    }
}

/**
 *
 */
int ebon_audio_t::load_bgm(const char* filename)
{
    if(_bgm)
    {
        printf("Loading [Already Loaded] BGM: %s\n", filename);
        if(_reserve_bgm)
            Mix_FreeMusic(_reserve_bgm);
        _reserve_bgm = Mix_LoadMUS(filename);
        strcpy(_loaded_bgm,filename);
        _have_reserve = true;
        _stored = true;
        if(_reserve_bgm != NULL)
        {
            return 0;
        }
    }
    else
    {
        printf("Loading [Nothing Loaded] BGM: %s\n", filename);
        _bgm = Mix_LoadMUS(filename);
        if(_bgm != NULL)
        return 0;
    }
    return -1;
}

/**
 *
 */
int ebon_audio_t::play_bgm(const char* filename, double volume, int loop)
{
    //However this function finishes, we should always be playing music
    _play_music = true;
    _paused = false;
    _volume = volume;
    _loops = loop;

    Mix_Music* sw_music;
    //If we've already pre-loaded this BGM
    if(_have_reserve && strcmp(_loaded_bgm,filename) == 0 )
    {
        //Simply switch to it
        _have_reserve = false;
        Mix_HaltMusic();
        if(_bgm)
            Mix_FreeMusic(_bgm); //Free whatever we were playing
        _bgm = _reserve_bgm; //Switch over to whatever was on reserve
        _reserve_bgm = NULL; //And leave no reserve music loaded
    }
    else
    {
        if(_bgm)
        {
            Mix_HaltMusic();
            Mix_FreeMusic(_bgm);
        }
        _bgm = Mix_LoadMUS(filename);
    }
}

/**
 *
 */
int ebon_audio_t::stop_bgm()
{
    Mix_HaltMusic();
}

/**
 *
 */
int ebon_audio_t::fade_out_bgm(unsigned ms)
{
    _fade_out = true;
    _fade_out_time = ms;
}

/**
 *
 */
int ebon_audio_t::fade_in_bgm(unsigned ms)
{
    _fade_in = true;
    _fade_in_time = ms;
}

/**
 *
 */
int ebon_audio_t::set_bgm_volume(double volume)
{
    _play_music = true;
    _volume = volume;
    if(_volume > MIX_MAX_VOLUME)
        _volume = MIX_MAX_VOLUME;
    if(_volume < 0)
        _volume = 0;
}

/**
 *
 */
void ebon_audio_t::store_bgm()
{
    _stored = true;
    //We can only create a shallow copy of the bgm.
    if(_reserve_bgm)
    {
        Mix_FreeMusic(_reserve_bgm);
    }
    _reserve_bgm = _bgm;
}

/**
 *
 */
void ebon_audio_t::restore_bgm()
{
    //This is just hot swapping the bgms out.
    if(_reserve_bgm && _stored)
    {
        _play_music = true;
        _paused = false;
        _have_reserve = false;
        _stored = false;
        Mix_HaltMusic();
        if(_bgm)
            Mix_FreeMusic(_bgm); //Free whatever we were playing
        _bgm = _reserve_bgm; //Switch over to whatever was on reserve
        _reserve_bgm = NULL; //And leave no reserve music loaded
    }
}

/**
 *
 */
void ebon_audio_t::pause_bgm()
{
    _paused = true;
}

/**
 *
 */
void ebon_audio_t::resume_bgm()
{
    _paused = false;
}

/**
 *
 */
int ebon_audio_t::play_se(const char* filename, double volume)
{
    printf("Playing Sound Effect: %s\n", filename);
    unsigned index = 0;
    for(index = 0; index < MAX_MIX_CHUNKS && _sfx[index] != NULL; ++index); //Find a chunk that isn't taken.
    //If we reached the end, report an error
    if(index == MAX_MIX_CHUNKS)
    {
        return -1;
    }
    printf("Loading sound into track %d\n",index);
    _sfx[index] = Mix_LoadWAV(filename);
    _play_sfx[index] = true;
    //Then set the volume
    if(volume<0)
        volume = 0;
    Mix_Volume(index,volume);
    return 0;
}

