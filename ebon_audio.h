/**
 * Copyright 2014, Andrew Dobson
 *
 * @file ebon_audio.h
 * @author Andrew Dobson
 */

#include "SDL.h"
#include "SDL_mixer.h"

#define MAX_MIX_CHUNKS  MIX_CHANNELS

//The main audio class, which internally uses SDL to handle audio.
//For the instance of this class, refer to Audio.
class ebon_audio_t
{
  public:
    ebon_audio_t();
    ~ebon_audio_t();

    //Initialization method.  Starts Mix with the appropriate parameters.
    int initialize();

    //Frame method.  This must be called every frame to ensure supplied
    //requests are fulfilled.  ALL of the following functions do not modify
    //the the audio until frame is called.
    void frame();

    //Load up a particular BGM given in filename, but do not start playing it
    int load_bgm(const char* filename);

    //Plays the music track given in filename
    int play_bgm(const char* filename, double volume = 128, int loop = -1);

    //Stops the bgm
    int stop_bgm();

    //Fade BGM in/out
    int fade_out_bgm(unsigned ms);
    int fade_in_bgm(unsigned ms);

    //Modify the currently playing music
    int set_bgm_volume(double volume);

    //Store music.  Helpful when one music track interrupts another briefly.
    void store_bgm();
    void restore_bgm();

    //Pausing and resuming
    void pause_bgm();
    void resume_bgm();

    //Sound effects must also be played
    int play_se(const char* filename, double volume = 128);

  private:

    //Pointers for dealing with music.  One will be the one played,
    //while the other is the one stored.  _bgm always refers to the
    //currently playing music.
    Mix_Music* _bgm;
    Mix_Music* _reserve_bgm;
    Mix_Music* _music_a;
    Mix_Music* _music_b;

    //Sound effect chunks
    Mix_Chunk*  _sfx[MAX_MIX_CHUNKS];
    bool        _play_sfx[MAX_MIX_CHUNKS]; //The internal channels the sfx play on.

    //Internal variables set which inform the update function what to do
    double _volume;           //The volume we should play music at.
    int _loops;               //How we should be looping the BGM.
    unsigned _fade_out_time;  //How many milliseconds should we be fading the bgm.
    unsigned _fade_in_time;   //How many milliseconds we should be fading in bgm.

    bool _play_music;     //Whether the music pointed to by _bgm should be playing.
    bool _paused;         //Whether the music should be paused.
    double _bgm_time;     //The time to begin executing _bgm at when we restore_bgm()
    bool _fade_out;       //Whether we should be fading out _bgm.
    bool _fade_in;        //Whether we should be fading in _bgm.
    bool _fading;         //Whether the music is fading right now.

    bool _have_reserve;   //This flag indicates whether there is a bgm pre-loaded in reserve.
    bool _stored;         //Indicates when the bgm in reserve was stored from _bgm.

    char _loaded_bgm[512];//The _bgm filename that has been loaded by load_bgm();
};

