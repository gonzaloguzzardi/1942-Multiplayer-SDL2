//
//  ClientMenuButtonTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/15/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClientMenuButtonTexture.hpp"
#include "ClientMenuUtil.hpp"

ClientMenuButtonTexture::ClientMenuButtonTexture(SDL_Window *menuWindow, std::string textureText, ClientMenuButtonTextureDelegate *buttonDelegate) : ClientMenuTextFieldTexture(menuWindow, textureText) {
    this->menuButtonEnabled = false;
    this->menuButtonDelegate = buttonDelegate;
}

ClientMenuButtonTexture::~ClientMenuButtonTexture() {
    
}

void ClientMenuButtonTexture::menuTextureRender(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip) {
    if (this->menuButtonEnabled == true) {
        // Draw multicolor lighting button.
        clock_t endTime = clock();
        clock_t clockTicksTaken = endTime - this->menuTextureTextFieldStartTime;
        double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
        
        if (timeInSeconds > 0.30) {
            this->menuTextureTextFieldStartTime = endTime;
            this->menuTextureTextFieldDrawLine = !this->menuTextureTextFieldDrawLine;
        }
        
        if (this->menuTextureTextFieldDrawLine == true) {
            ClientMenuTexture::menuTextureRender(x, y, clip, angle, center, flip);
            
            SDL_Color borderColor =  ClientMenuUtils::clientMenuTextColor();
            SDL_Rect outlineRect = { x - 8, y, this->menuTextureWidth + 16, this->menuTextureHeight };
            SDL_SetRenderDrawColor(this->menuTextureRenderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
            SDL_RenderDrawRect(this->menuTextureRenderer, &outlineRect);
        }
    } else {
        // Draw gray static button.
        ClientMenuTexture::menuTextureRender(x, y, clip, angle, center, flip);
        SDL_Color borderColor =  ClientMenuUtils::clientMenuDisableTextColor();
        
        SDL_Rect outlineRect = { x - 8, y, this->menuTextureWidth + 16, this->menuTextureHeight };
        SDL_SetRenderDrawColor(this->menuTextureRenderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        SDL_RenderDrawRect(this->menuTextureRenderer, &outlineRect);
    }
}

SDL_Color ClientMenuButtonTexture::menuTextureGetTextColor() {
    if (this->menuButtonEnabled == false) {
        return ClientMenuUtils::clientMenuDisableTextColor();
    }
    
    return ClientMenuUtils::clientMenuTextColorSelected();
}

void ClientMenuButtonTexture::menuButtonSetEnabled(bool enabled) {
    this->menuButtonEnabled = enabled;
}

void ClientMenuButtonTexture::menuTextureTextFieldHandlerMouseEvent(SDL_Event *mouseEvent) {
    bool mouseEventInside = this->menuTextureTextFieldEventInside(mouseEvent);
    
    if (this->menuButtonEnabled == true && mouseEventInside) {
        this->menuButtonDelegate->menuButtonTouched();
        this->menuButtonEnabled = false;
    }
}
