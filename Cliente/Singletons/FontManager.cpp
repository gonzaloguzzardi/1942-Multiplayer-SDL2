#include "FontManager.h"

FontManager* FontManager::s_pInstance = 0;

void FontManager::init()
{
    if (TTF_Init() == -1) {
        printf("FontManager - SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }
    m_font = NULL;
    loadfont(DEFAULT_FONT_PATH,DEFAULT_FONT_SIZE);
}

bool FontManager::loadfont(char* file, int ptsize)
{
    TTF_Font* tmpfont;
    tmpfont = TTF_OpenFont(file, ptsize);
    if (tmpfont == NULL)
    {
        printf("Unable to load font: %s %s \n", file, TTF_GetError());
        return false;
    }
    if(m_font)
        TTF_CloseFont(m_font);
    m_font = tmpfont;
    return true;
}

int FontManager::textSize(const char* text,int *h,int* w)
{
    return TTF_SizeText(m_font,text,w,h);
}

SDL_Texture* FontManager::drawtext(char fgR, char fgG, char fgB, char fgA,
                                   char bgR, char bgG, char bgB, char bgA, const char text[], textquality quality)
{
    if (m_font == NULL) {
        return NULL;
    }
    
    SDL_Color tmpfontcolor = { static_cast<Uint8>(fgR), static_cast<Uint8>(fgG), static_cast<Uint8>(fgB),static_cast<Uint8>(fgA) };
    SDL_Color tmpfontbgcolor = { static_cast<Uint8>(bgR), static_cast<Uint8>(bgG), static_cast<Uint8>(bgB), static_cast<Uint8>(bgA)};
    SDL_Surface* resulting_text;
    
    if (quality == solid) {
        resulting_text = TTF_RenderText_Solid(m_font, text, tmpfontcolor);
    }
    else if (quality == shaded) {
        resulting_text = TTF_RenderText_Shaded(m_font, text, tmpfontcolor, tmpfontbgcolor);
    }
    else if (quality == blended) {
        resulting_text = TTF_RenderText_Blended(m_font, text, tmpfontcolor);
    }
    
    return SDL_CreateTextureFromSurface(Game::Instance()->getRenderer(), resulting_text);
}

