//
//  ClientMenuComboBoxOptionTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/15/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClientMenuComboBoxOptionTexture.hpp"
#include "ClientMenuUtil.hpp"

// Constructor and Destructor.
ClientMenuComboBoxOptionTexture::ClientMenuComboBoxOptionTexture(SDL_Window *menuWindow, std::string textureText) : ClientMenuTextFieldTexture(menuWindow, textureText) {
}

ClientMenuComboBoxOptionTexture::~ClientMenuComboBoxOptionTexture() {
    
}

void ClientMenuComboBoxOptionTexture::menuTextureRender(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    ClientMenuTextFieldTexture::menuTextureRender(x, y, clip, angle, center, flip);
    
    //Draw blue horizontal line
    SDL_Color textColor =  ClientMenuUtils::clientMenuTextColorSelected();
    SDL_SetRenderDrawColor(this->menuTextureRenderer, textColor.r, textColor.g, textColor.b, textColor.a);
    SDL_RenderDrawLine(this->menuTextureRenderer, x, y + this->menuTextureHeight, x + this->menuTextureWidth, y + this->menuTextureHeight);
}