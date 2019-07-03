/**
 * @author Andrew Dobson (www.doctorandrewdobson.com)
 * @file ebon.h
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 * not claim that you wrote the original software. If you use this
 * software in a product, an acknowledgment in the product documentation
 * would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 *
 * This file represents the main EBON infrastructure.  Ideally, a user only
 * has to include this single file to gain access to all of EBON's functionality,
 * granted that EBON Libraries and headers are installed properly.
 */

#include "ebon_audio.h"
#include "ebon_graphics.h"

// Maybe I do want to put everything into an Ebon namespace?

namespace EBON
{

// Here are the actual EBON instances to be used in your application.
ebon_audio_t AUDIO;
ebon_graphics_t GRAPHICS;

// There should also be some other global information which will be read and
// can be used by other parts of the library
SDL_Event global_event;
unsigned E_RES_WIDTH = 1280;
unsigned E_RES_HEIGHT = 760;


// XML Readers (is there something simple I can import?)


//=-=-=-=-=-=-=-=-=-=-=
// Basic Utilities
//=-=-=-=-=-=-=-=-=-=-=

void get_timestring( char* timestring, unsigned offset = 0 )
{
    unsigned ticks = SDL_GetTicks();
    unsigned seconds = (ticks - offset)/1000;
    //Quick offset check.
    if( offset > ticks )
        seconds = 0;
    sprintf(timestring, "%d:%d%d:%d%d",
                seconds/3600,
                (seconds/600)%6,
                (seconds/60)%10,
                (seconds/10)%6,
                seconds%10);
}

//=-=-=-=-=-=-=-=-=-=-=
// Initialization
//=-=-=-=-=-=-=-=-=-=-=

bool EBON_init_audio()
{
    //Empty for now, might have some XML parameters I want
    return true;
}

bool EBON_init_video()
{
    //Will definitely want to load up XML parameters
    std::string window_name = "EBON DEFAULT WINDOW"; //TODO read in

    // Initialize the window :: TODO: Window Name and Resolution to be read-in.
    GRAPHICS.initialize(window_name.c_str(), E_RES_WIDTH, E_RES_HEIGHT);

    return true;
}

bool EBON_init()
{
    // Always need to Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    // Then call initialization on libraries
    bool good_init = true;
    good_init &= EBON_init_audio();
    good_init &= EBON_init_video();

    return good_init;
}

void EBON_quit()
{
    SDL_Quit();
}

//=-=-=-=-=-=-=-=-=-=-=
// Main Loop Function
//=-=-=-=-=-=-=-=-=-=-=






} //end namespace
