#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "ebon.h"

#undef main

/**
 * MAIN
 */
int main(int argc, char* argv[])
{
    EBON::EBON_init();

    //DEBUG: Set up some text
    EBON::GRAPHICS.register_font("LCALLIG",36);
    EBON::GRAPHICS.register_font("consolab",26);

    //Render static text
    if( !EBON::GRAPHICS.use_font("LCALLIG",36) )
        return -1;
    auto txt = EBON::GRAPHICS.rnd_text( "Use the arrow keys to scroll the map.", EBON::COLOR["orange"] );
    if( !EBON::GRAPHICS.use_font("consolab",26) )
        return -1;

    auto bg = EBON::GRAPHICS.rnd_img("img/AnnotatedMap.png");
    double dx = EBON::E_RES_WIDTH;
    char timestring[10];
    EBON::AUDIO.play_se("audio/Cancel_X.wav", 10);
    while(EBON::global_event.type != SDL_QUIT)
    {
        EBON::GRAPHICS.render_rect( {0,0,EBON::E_RES_WIDTH,EBON::E_RES_HEIGHT}, EBON::COLOR["brick"] );
        EBON::GRAPHICS.render_sub( bg, {800,400,EBON::E_RES_WIDTH-100,EBON::E_RES_HEIGHT-100}, 50, 50 );
        EBON::GRAPHICS.render(txt, dx, 10);

        EBON::get_timestring(timestring);
        EBON::GRAPHICS.render( EBON::GRAPHICS.rnd_text( timestring, EBON::COLOR["yellow"]), 135, EBON::E_RES_HEIGHT-40, EBON::JUSTIFY::RIGHT );

        EBON::GRAPHICS.frame();
        EBON::AUDIO.frame();

        dx -= 1.2;

        bool has_event = SDL_PollEvent(&EBON::global_event);
    }

    EBON::EBON_quit();
    return 0;
}

