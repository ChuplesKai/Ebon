#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "ebon_audio.h"
#include "ebon_graphics.h"

#undef main

/**
 * MAIN
 */
int main(int argc, char* argv[])
{
    // Always need to Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    // Initialize EBON modules
    ebon_audio_t AUDIO;
    ebon_graphics_t GRAPHICS;
    // Initialize the window
    GRAPHICS.initialize("Test Window", 1080, 720);

    //DEBUG: Set up some text
    GRAPHICS.register_font("Amperzand",36);
    GRAPHICS.register_font("Amperzand",26);

    //Render static text
    if( !GRAPHICS.use_font("Amperzand",36) )
        return -1;
    auto txt = GRAPHICS.rnd_text( "Scrolling Text of Awesome", EB_COL["salmon"] );
    if( !GRAPHICS.use_font("Amperzand",26) )
        return -1;

    char timetext[50];

    auto bg = GRAPHICS.rnd_img("img/AnnotatedMap.png");

    SDL_Event global_event;
    double dx = -400;
    AUDIO.play_se("audio/Cancel_X.wav", 10);
    while(global_event.type != SDL_QUIT)
    {
        GRAPHICS.render_sub( bg, {800,400,780,520}, 150, 100 );

        GRAPHICS.render(txt, dx, 30);

        sprintf(timetext, "%d:%d", SDL_GetTicks()/60000, (SDL_GetTicks()/1000)%60);
        GRAPHICS.render( GRAPHICS.rnd_text(timetext, EB_COL["emerald"]), 30, 720-50 );

        GRAPHICS.frame();
        AUDIO.frame();

        dx += 2.0;

        bool has_event = SDL_PollEvent(&global_event);
    }

    /* Audio Test
    char error[200];
    AUDIO.play_bgm("Battling Aces.wav", 10.0);
    idle(AUDIO);
    AUDIO.play_se("Cancel_X.wav", 10);
    idle(AUDIO);
    AUDIO.load_bgm("Battle like an Ace.wav");
    idle(AUDIO);
    AUDIO.restore_bgm();
    idle(AUDIO);
    AUDIO.fade_out_bgm(8000);
    idle(AUDIO);
    idle(AUDIO);
    */

    SDL_Quit();
    return 0;
}

