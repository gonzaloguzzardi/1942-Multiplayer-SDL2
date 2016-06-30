//
//  ClientMenuTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include <SDL2/SDL_image.h>

#include "ClientMenuTexture.hpp"

ClientMenuTexture::ClientMenuTexture(SDL_Window *menuWindow) {
    //Initialize
    this->menuTexture = NULL;
    this->menuTextureRenderer = SDL_GetRenderer(menuWindow);
    this->menuTextureWidth = 0;
    this->menuTextureHeight = 0;
    this->menuTextureLoaded = false;
}

ClientMenuTexture::~ClientMenuTexture() {
    if (this->menuTexture != NULL) {
        SDL_DestroyTexture(this->menuTexture);
    }
}

void ClientMenuTexture::menuTextureRender(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip) {
    this->menuTextureX = x;
    this->menuTextureY = y;
    
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, this->menuTextureWidth, this->menuTextureHeight };
    
    //Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    
    //Render to screen
    SDL_RenderCopyEx(this->menuTextureRenderer, this->menuTexture, clip, &renderQuad, angle, center, flip);
}

int ClientMenuTexture::menuTextureGetWidth()
{
    return this->menuTextureWidth;
}

int ClientMenuTexture::menuTextureGetHeight()
{
    return this->menuTextureHeight;
}

bool ClientMenuTexture::menuTextureGetLoaded() {
    return this->menuTextureLoaded;
}

SDL_Texture *ClientMenuTexture::menuTextureGetTexture() {
    return this->menuTexture;
}

void ClientMenuTexture::menuTextureSetModulation(SDL_Color modulationColor) {
    SDL_SetTextureColorMod(this->menuTexture, modulationColor.r, modulationColor.g, modulationColor.b);
}
