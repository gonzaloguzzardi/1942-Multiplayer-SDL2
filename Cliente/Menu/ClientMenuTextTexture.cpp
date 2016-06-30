//
//  ClientMenuTextTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClientMenuTextTexture.hpp"
#include "ClientMenuUtil.hpp"

ClientMenuTextTexture::ClientMenuTextTexture(SDL_Window *menuWindow, std::string textureText) : ClientMenuTexture(menuWindow) {
    this->menuTextureLoaded = true;
    this->menuTextureSelected = false;
    this->menuTextureFont = NULL;
    this->menuTextureText = textureText;
    
    if (TTF_Init() == -1) {
        printf("ClientMenuTextTexture.cpp - SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        this->menuTextureLoaded = false;
    }
}

ClientMenuTextTexture::~ClientMenuTextTexture() {
    TTF_CloseFont(this->menuTextureFont);
    
    SDL_DestroyTexture(this->menuTexture);
    
    TTF_Quit();
}

void ClientMenuTextTexture::menuTextureSetTextFont(int fontSize) {
    TTF_CloseFont(this->menuTextureFont);
    this->menuTextureFont = this->menuTextureGetTextFont(fontSize);
    this->menuTextureLoaded = true;
    
    if (this->menuTextureFont == NULL) {
        printf("ClientMenuTextTexture.cpp - Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        this->menuTextureLoaded = false;
    }
}

void ClientMenuTextTexture::menuTextureCreateTextSurface() {
    //Render text surface
    SDL_Color textColor = this->menuTextureGetTextColor();
    SDL_Surface *textSurface = TTF_RenderText_Solid(this->menuTextureFont, this->menuTextGetTextString().c_str(), textColor);
    if (textSurface == NULL) {
        printf("ClientMenuTextTexture.cpp - Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        this->menuTextureLoaded = false;
    } else {
        //Create texture from surface pixels
        SDL_DestroyTexture(this->menuTexture);
        this->menuTexture = SDL_CreateTextureFromSurface(this->menuTextureRenderer, textSurface);
        
        if (this->menuTexture == NULL) {
            printf("ClientMenuTextTexture.cpp - Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            this->menuTextureLoaded = false;
        } else {
            //Get image dimensions
            this->menuTextureWidth = textSurface->w;
            this->menuTextureHeight = textSurface->h;
        }
        
        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    
    
    //Return success
    this->menuTextureLoaded =  this->menuTexture != NULL;
}

SDL_Color ClientMenuTextTexture::menuTextureGetTextColor() {
    if (this->menuTextureSelected == true) {
        return ClientMenuUtils::clientMenuTextColorSelected();
    } else {
        return ClientMenuUtils::clientMenuTextColor();
    }
}

TTF_Font *ClientMenuTextTexture::menuTextureGetTextFont(int fontSize) {
    return ClientMenuUtils::clientMenuFont(fontSize);
}

std::string ClientMenuTextTexture::menuTextGetTextString() {
    return this->menuTextureText;
}
