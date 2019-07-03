/**
 * @author Andrew Dobson (www.doctorandrewdobson.com)
 * @file ebon_graphics.h
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ostream>
#include <vector>
#include <map>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

namespace EBON
{

// Standard Colors Map
std::map< std::string, SDL_Color > COLOR =
{
    {"white", {255,255,255} },
    {"gray", {108,108,108} },
    {"black", {0,0,0} },
    {"blue", {0,0,230} },
    {"red", {230,0,0} },
    {"green", {0,230,0} },
    {"cyan", {0,230,230} },
    {"emerald", {0,130,130} },
    {"orange", {230,130,0} },
    {"yellow", {255,255,0} },
    {"magenta", {230,0,230} },
    {"pink", {255,180,190} },
    {"salmon", {155,40,50} },
    {"brick", {110,20,0} },
    {"navy", {0,20,110} },
    {"forest", {10,110,10} },
    {"brown", {110,80,0} }
};

enum JUSTIFY {LEFT, CENTER, RIGHT};

//The main graphics class, which internally uses SDL to handle
// window management and drawing to windows.
//For the instance of this class, refer to Graphics.
class ebon_graphics_t
{
  public:
    // Standard constructor/destructor stuff
    ebon_graphics_t();
    ~ebon_graphics_t();

    // Initialization
    void initialize( std::string title, unsigned inwidth = 960, unsigned inheight = 544 );

    //Frame method.  This must be called every frame to ensure supplied
    //requests are fulfilled.
    void frame();

    //Generic Texture Render
    void render( SDL_Texture* t, int x=0, int y=0, JUSTIFY js = JUSTIFY::LEFT );
    void render_sub( SDL_Texture* t, std::vector<unsigned> subrect, int x=0, int y=0, JUSTIFY js = LEFT );
    //Basic Rectangle Render
    void render_rect( std::vector<unsigned> r, SDL_Color& color );

    //Generate a Texture from an image
    SDL_Texture* rnd_img( std::string image, SDL_Color color = {255,255,255} );

    //Generate a Text Texture
    SDL_Texture* rnd_text( std::string text, SDL_Color& color );

    bool use_font( std::string fontname, unsigned sz ); //Switch to a registered font
    bool register_font( std::string fontname, unsigned sz );//Register a font

  private:
    SDL_Window *_window = NULL;  //The only window we want to have open for basics
    SDL_Renderer *_ren = NULL;   //The renderer which creates all of our textures and bitmaps

    std::map< std::string, TTF_Font* > _registered_fonts;

    TTF_Font *_font = NULL;
    std::string _fontfile = "Amperzand.ttf";

    unsigned _wwidth;
    unsigned _wheight;

    //Internal surface creation from a template
    SDL_Surface* CreateSurface(Uint32 flags, int width, int height, const SDL_Surface* display);
    std::string fontstring( std::string fontname, unsigned sz );
};

} // end namespace
