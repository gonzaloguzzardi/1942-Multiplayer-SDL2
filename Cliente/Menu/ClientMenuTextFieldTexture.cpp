//
//  ClientMenuTextFieldTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/15/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include <algorithm>
#include <unistd.h>
#include <sstream>
#include <string>
#include <list>

#include "ClientMenuTextFieldTexture.hpp"
#include "ClientMenuUtil.hpp"

ClientMenuTextFieldTexture::ClientMenuTextFieldTexture(SDL_Window *menuWindow, std::string textureText) : ClientMenuTextTexture(menuWindow, textureText) {
    this->menuTextureTextFieldStartTime = clock();
    this->menuTextureTextFieldDrawLine = true;
    this->menuTextureSelected = false;
}

ClientMenuTextFieldTexture::~ClientMenuTextFieldTexture() {
}

void ClientMenuTextFieldTexture::menuTextureRender(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip) {
    clock_t endTime = clock();
    clock_t clockTicksTaken = endTime - this->menuTextureTextFieldStartTime;
    double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
    
    if (timeInSeconds > 0.30) {
        this->menuTextureTextFieldStartTime = endTime;
        this->menuTextureTextFieldDrawLine = !this->menuTextureTextFieldDrawLine;
    }
    
    if (this->menuTextureSelected == true) {
        if (this->menuTextureTextFieldDrawLine == true) {
            // Draw the text.
            ClientMenuTexture::menuTextureRender(x, y, clip, angle, center, flip);
            
            //Draw blue horizontal line
            SDL_Color textColor =  ClientMenuUtils::clientMenuTextColor();
            SDL_SetRenderDrawColor(this->menuTextureRenderer, textColor.r, textColor.g, textColor.b, textColor.a);
            SDL_RenderDrawLine(this->menuTextureRenderer, x, y + this->menuTextureHeight, x + this->menuTextureWidth, y + this->menuTextureHeight);
        }
    } else {
        ClientMenuTexture::menuTextureRender(x, y, clip, angle, center, flip);
        
        if (this->menuTextureTextFieldDrawLine == true) {
            //Draw blue horizontal line
            SDL_Color textColor =  ClientMenuUtils::clientMenuTextColorSelected();
            
            SDL_SetRenderDrawColor(this->menuTextureRenderer, textColor.r, textColor.g, textColor.b, textColor.a);
            SDL_RenderDrawLine(this->menuTextureRenderer, x, y + this->menuTextureHeight, x + this->menuTextureWidth, y + this->menuTextureHeight);
        }
    }
}

bool ClientMenuTextFieldTexture::menuTextureTextFieldEventInside(SDL_Event *mouseEvent) {
    int mouseEventX = mouseEvent->motion.x;
    int mouseEventY = mouseEvent->motion.y;
    
    int textureMinX = this->menuTextureX;
    int textureMaxX = this->menuTextureX + this->menuTextureWidth;
    
    int textureMinY = this->menuTextureY;
    int textureMaxY = this->menuTextureY + this->menuTextureHeight;
    
    if (mouseEventX > textureMinX && mouseEventX < textureMaxX &&
        mouseEventY > textureMinY && mouseEventY < textureMaxY) {
        return true;
    }
    
    return false;
}

void ClientMenuTextFieldTexture::menuTextureTextFieldHandlerMouseEvent(SDL_Event *mouseEvent) {
    bool mouseEventInside = this->menuTextureTextFieldEventInside(mouseEvent);
    
    if (mouseEventInside == true && this->menuTextureSelected == false) {
        this->menuTextureSelected = true;
    } else {
        this->menuTextureSelected = false;
    }
}

bool ClientMenuTextFieldTexture::menuTextureTextFieldInputIsBackSpace(SDL_Event *inputEvent) {
    if (inputEvent->type == SDL_KEYDOWN && inputEvent->key.keysym.sym == SDLK_BACKSPACE) {
        return true;
    }
    
    return false;
}

bool ClientMenuTextFieldTexture::menuTextureTextFieldInputIsReturn(SDL_Event *inputEvent) {
    if (inputEvent->type == SDL_KEYDOWN && inputEvent->key.keysym.sym == SDLK_RETURN) {
        return true;
    }
    
    return false;
}

void ClientMenuTextFieldTexture::menuTextureTextFieldHanldlerInputEvent(SDL_Event *inputEvent) {
    bool isBackSpace = this->menuTextureTextFieldInputIsBackSpace(inputEvent);
    bool isReturn = this->menuTextureTextFieldInputIsReturn(inputEvent);
    
    if (isReturn == true) {
        this->menuTextureSelected = false;
        return;
    }
    
    if (isBackSpace && this->menuTextureText.length() > 0) {
        this->menuTextureText.pop_back();
        
        if (this->menuTextureText.length() == 0) {
            this->menuTextureText.append(" ");
        }
    } else {
        // An alphanumeric key pressed.
        std::string newString = std::string(inputEvent->text.text);
        std::list<std::string> allowedCharacters = ClientMenuUtils::clientMenuAllowedCharacters();
        bool allowedCharacter = (std::find(allowedCharacters.begin(), allowedCharacters.end(), newString) != allowedCharacters.end());

        if (allowedCharacter == true && this->menuTextureText.length() < 23) {
            if (this->menuTextureText == " ") {
                this->menuTextureText = "";
            }
            
            this->menuTextureText.append(inputEvent->text.text);
        }
    }
}

bool ClientMenuTextFieldTexture::menuTextureTextFieldIsSelected() {
    return this->menuTextureSelected;
}
