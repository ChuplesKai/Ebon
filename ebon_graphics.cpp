/**
 * @author Andrew Dobson (www.doctorandrewdobson.com)
 * @file ebon_graphics.cpp
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

#include <cstdlib>
#include <iostream>
#include <math.h>

#include "ebon_graphics.h"

using namespace std;
using namespace EBON;

/**
 *
 */
ebon_graphics_t::ebon_graphics_t()
{
}

/**
 *
 */
ebon_graphics_t::~ebon_graphics_t()
{
    //Close out all of our fonts
    for( auto e : _registered_fonts )
    {
    	TTF_CloseFont(e.second);
    }

    //Stop Image stuffs
    IMG_Quit();
}


/**
 *
 */
void ebon_graphics_t::initialize( string title, unsigned inwidth, unsigned inheight )
{
    // Attempt to initialize Font Support
    if( TTF_Init() != 0 )
    {
        cout << "Error initializing Fonts\n";
        exit(2);
    }

    // Overwrite width/height parameters
    _wwidth = inwidth;
    _wheight = inheight;

    // Generate The Initial Window
    _window = SDL_CreateWindow
    (
        title.c_str(),                    // window title
        SDL_WINDOWPOS_UNDEFINED,          // initial x position
        SDL_WINDOWPOS_UNDEFINED,          // initial y position
        _wwidth,                          // width, in pixels
        _wheight,                         // height, in pixels
        SDL_WINDOW_OPENGL // flags - see below
    );

    // Check that the window was successfully made
    if(_window == NULL)
    {
        // In the event that the window could not be made...
        cerr << "Could not create window: " << SDL_GetError() << "\n";
        exit(3);
    }

    _ren = SDL_CreateRenderer(_window, -1,	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (_ren == NULL)
    {
        cerr << "Renderer failed to create: " << SDL_GetError() << "\n";
        exit(3);
    }
}

/**
 *
 */
void ebon_graphics_t::frame()
{
    SDL_RenderPresent(_ren);
    SDL_RenderClear(_ren);
}

/**
 *
 */
SDL_Surface* ebon_graphics_t::CreateSurface(Uint32 flags, int width, int height, const SDL_Surface* display)
{
    const SDL_PixelFormat fmt = *(display->format);
    return SDL_CreateRGBSurface(flags, width, height,
                fmt.BitsPerPixel, fmt.Rmask, fmt.Gmask, fmt.Bmask, fmt.Amask );
}

/**
 *
 */
void ebon_graphics_t::render( SDL_Texture* t, int x, int y, JUSTIFY js )
{
    SDL_Rect rct;
    int iW, iH;
    // Get some data about what we're trying to render
    SDL_QueryTexture(t, NULL, NULL, &iW, &iH);
    //Change coordinates for non-left justification
    if(js == JUSTIFY::RIGHT)
    {
        rct.x = x - iW;
    }
    else if(js == JUSTIFY::CENTER)
    {
        rct.x = x - iW/2;
    }
    else
    {
        rct.x = x;
    }
    rct.y = y;
    rct.w = iW;
    rct.h = iH;
    SDL_RenderCopy( _ren, t, NULL, &rct );
}

/**
 *
 */
void ebon_graphics_t::render_sub( SDL_Texture* t, std::vector<unsigned> subrect, int x, int y, JUSTIFY js )
{
    SDL_Rect srct, drct;
    int iW, iH;
    if( js == JUSTIFY::RIGHT )
    {
        drct.x = x - subrect[2];
    }
    else if(js == JUSTIFY::CENTER)
    {
        drct.x = x - subrect[2]/2;
    }
    else
    {
        drct.x = x;
    }

    //Destination: size must correspond to source
    drct.y = y;
    drct.w = subrect[2];
    drct.h = subrect[3];
    //Source size is from parameter
    srct.x = subrect[0];
    srct.y = subrect[1];
    srct.w = subrect[2];
    srct.h = subrect[3];
    //Then do the actual render
    SDL_RenderCopy( _ren, t, &srct, &drct );
}

/**
 *
 */
void ebon_graphics_t::render_rect( std::vector<unsigned> r, SDL_Color& color )
{
    SDL_Rect srct;
    srct.x = r[0];
    srct.y = r[1];
    srct.w = r[2];
    srct.h = r[3];
    SDL_SetRenderDrawColor(_ren, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(_ren, &srct);
    SDL_SetRenderDrawColor(_ren, 0, 0, 0, 255);
}

/**
 *
 */
SDL_Texture* ebon_graphics_t::rnd_img( std::string image, SDL_Color color )
{
    SDL_Texture *tex = IMG_LoadTexture(_ren, image.c_str());
    if( tex != NULL )
        SDL_SetTextureColorMod( tex, color.r, color.g, color.b );
    return tex;
}

/**
 *
 */
SDL_Texture* ebon_graphics_t::rnd_text( std::string text, SDL_Color& color )
{
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	int txtW, txtH;
	SDL_Surface *coloredt = TTF_RenderText_Blended(_font, text.c_str(), color);
	SDL_Surface *blackt = TTF_RenderText_Blended(_font, text.c_str(), COLOR["black"]);
	SDL_SetSurfaceAlphaMod( blackt, 150 );
	txtW = blackt->w;
	txtH = blackt->h;
	int offset = txtH/20;
	SDL_Surface *fsurf = CreateSurface( 0, txtW+offset, txtH+offset, coloredt);

    SDL_Rect srct, drct;
    srct.w = drct.w = txtW;
    srct.h = drct.h = txtH;
    srct.x = srct.y = 0;
    drct.x = drct.y = offset;

    SDL_BlitSurface(blackt, &srct, fsurf, &drct); //Should be the right way
    drct.x = drct.y = 0;
    SDL_BlitSurface(coloredt, &srct, fsurf, &drct);
    //Final Texture
    SDL_Texture *texture = SDL_CreateTextureFromSurface(_ren, fsurf);

	//Clean up the surface and font
	SDL_FreeSurface(blackt);
	SDL_FreeSurface(coloredt);
	SDL_FreeSurface(fsurf);

	return texture;
}

/**
 *
 */
std::string ebon_graphics_t::fontstring( std::string fontname, unsigned sz )
{
    char s[3];
    std::string r(fontname);
    r = r + std::string("_");
    sprintf(s, "%d", sz);
    r = r + std::string(s);
    return r;
}

/**
 *
 */
bool ebon_graphics_t::use_font( std::string fontname, unsigned sz )
{
    //See if the font has been registered yet.
    std::string req_font = fontstring( fontname, sz );
    if( _registered_fonts.find(req_font) != _registered_fonts.end() )
    {
        _font = _registered_fonts[req_font];
        return true;
    }
    //If the font hasn't been registered, complain
    std::cerr << "Could not find registered font <" << req_font << ">\n";
    _font = NULL;
    return false;
}

/**
 *
 */
bool ebon_graphics_t::register_font( std::string fontname, unsigned sz )
{
    std::string reg_font = fontstring( fontname, sz );
    //Attempt to open the font
    std::string ff("C:\\Windows\\Fonts\\");
    ff = ff + fontname + std::string(".ttf");
    TTF_Font *font = TTF_OpenFont(ff.c_str(), sz);
    if( font == NULL )
    {
        std::cerr << "Error: could not register font <" << fontname << ">\n";
        return false;
    }
    //If there's a font, register it
    _registered_fonts[reg_font] = font;
    std::cout << "Registering font < " << reg_font << " >\n";
    return true;
}

