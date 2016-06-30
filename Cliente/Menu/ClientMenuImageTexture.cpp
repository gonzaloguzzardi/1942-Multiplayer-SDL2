//
//  ClientMenuImageTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include <SDL2/SDL_image.h>

#include "ClientMenuImageTexture.hpp"

ClientMenuImageTexture::ClientMenuImageTexture(SDL_Window *menuWindow)  : ClientMenuTexture(menuWindow) {
    if (!(IMG_Init(IMG_INIT_PNG))) {
        printf("ClientMenuImageTexture.cpp - SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        this->menuTextureLoaded = false;
    }
}

ClientMenuImageTexture::~ClientMenuImageTexture() {
    IMG_Quit();
}

void ClientMenuImageTexture::menuTextureLoadFromFile(const char *filePath) {
    //The final texture
    SDL_Texture *newTexture = NULL;
    
    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(filePath);
    
    if (loadedSurface == NULL) {
        printf("ClientMenuImageTexture.cpp - Unable to load image %s! SDL_image Error: %s\n", filePath, IMG_GetError());
        this->menuTextureLoaded = false;
    } else {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF ));
        
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(this->menuTextureRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("ClientMenuImageTexture.cpp - Unable to create texture from %s! SDL Error: %s\n", filePath, SDL_GetError());
            SDL_FreeSurface(loadedSurface);
            this->menuTextureLoaded = false;
        } else {
            //Get image dimensions
            this->menuTextureWidth = loadedSurface->w;
            this->menuTextureHeight = loadedSurface->h;
        }
        
        SDL_FreeSurface(loadedSurface);
    }
    
    //Return success
    this->menuTexture = newTexture;
    
    this->menuTextureLoaded = this->menuTexture != NULL;
}
