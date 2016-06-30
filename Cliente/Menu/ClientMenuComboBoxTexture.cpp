//
//  ClientMenuComboBoxTexture.cpp
//  Menu
//
//  Created by Gaston Montes on 6/15/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClientMenuComboBoxTexture.hpp"
#include "ClientMenuUtil.hpp"

// Constructor and destructor.
ClientMenuComboBoxTexture::ClientMenuComboBoxTexture(SDL_Window *menuWindow, std::string textureText, std::list<std::string> *optionsList) : ClientMenuTextFieldTexture(menuWindow, textureText) {
    this->menuComboBoxWindow = menuWindow;
    this->menuComboBoxSelectedIndex = -1;
    this->menuComboBoxEnabled = false;
    
    if (optionsList->size() > 0) {
        this->menuComboBoxEnabled = true;
        this->menuComboBoxTextList = optionsList;
        this->menuComboBoxOptionsList = new std::list<ClientMenuComboBoxOptionTexture *>();
        
        std::list<std::string>::iterator iterator = this->menuComboBoxTextList->begin();
        for (*iterator; iterator != this->menuComboBoxTextList->end(); ++iterator) {
            ClientMenuComboBoxOptionTexture *teamOption = new ClientMenuComboBoxOptionTexture(this->menuComboBoxWindow, iterator->c_str());
            
            SDL_Rect imageRect;
            imageRect.x = 0;
            imageRect.y = 0;
            SDL_GetWindowSize(this->menuComboBoxWindow, &imageRect.w, &imageRect.h);
            
            teamOption->menuTextureSetTextFont(imageRect.w / 25);
            teamOption->menuTextureCreateTextSurface();
            this->menuComboBoxOptionsList->push_back(teamOption);
        }
    }
}

ClientMenuComboBoxTexture::~ClientMenuComboBoxTexture() {
    if (this->menuComboBoxEnabled == true) {
        std::list<ClientMenuComboBoxOptionTexture *>::iterator iterator = this->menuComboBoxOptionsList->begin();
        for (*iterator; iterator != this->menuComboBoxOptionsList->end(); ++iterator) {
            delete *iterator;
        }
        
        delete this->menuComboBoxOptionsList;
    }
}

// Rendering functions.
void ClientMenuComboBoxTexture::menuComboBoxRenderOptions() {
    int initialY = this->menuTextureY + this->menuTextureHeight;
    
    std::list<ClientMenuComboBoxOptionTexture *>::iterator iterator = this->menuComboBoxOptionsList->begin();
    for (*iterator; iterator != this->menuComboBoxOptionsList->end(); ++iterator) {
        ClientMenuComboBoxOptionTexture *teamOption = *iterator;
        
        int initialX = this->menuTextureX + this->menuTextureWidth / 2 - teamOption->menuTextureGetWidth() / 2;
        teamOption->menuTextureRender(initialX, initialY);
        
        initialY += this->menuTextureHeight - 1;
    }
    
    this->menuTextureHeight = initialY;
}

void ClientMenuComboBoxTexture::menuTextureRender(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip) {
    if (this->menuComboBoxEnabled == true) {
        clock_t endTime = clock();
        clock_t clockTicksTaken = endTime - this->menuTextureTextFieldStartTime;
        double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
        
        if (timeInSeconds > 0.30) {
            this->menuTextureTextFieldStartTime = endTime;
            this->menuTextureTextFieldDrawLine = !this->menuTextureTextFieldDrawLine;
        }
        
        if (this->menuTextureSelected == true) {
            if (this->menuTextureTextFieldDrawLine == true) {
                ClientMenuTexture::menuTextureRender(x, y, clip, angle, center, flip);
                
                SDL_Rect outlineRect = { x - 8, y, this->menuTextureWidth + 16, this->menuTextureHeight };
                SDL_Color borderColor =  ClientMenuUtils::clientMenuTextColor();
                SDL_SetRenderDrawColor(this->menuTextureRenderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
                SDL_RenderDrawRect(this->menuTextureRenderer, &outlineRect);
                
                this->menuComboBoxRenderOptions();
            }
        } else {
            ClientMenuTexture::menuTextureRender(x, y, clip, angle, center, flip);
            
            if (this->menuTextureTextFieldDrawLine == true) {
                SDL_Rect outlineRect = { x - 8, y, this->menuTextureWidth + 16, this->menuTextureHeight };
                SDL_Color borderColor =  ClientMenuUtils::clientMenuTextColorSelected();
                SDL_SetRenderDrawColor(this->menuTextureRenderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
                SDL_RenderDrawRect(this->menuTextureRenderer, &outlineRect);
            }
        }
    } else {
        ClientMenuTexture::menuTextureRender(x, y, clip, angle, center, flip);
        
        SDL_Rect outlineRect = { x - 8, y, this->menuTextureWidth + 16, this->menuTextureHeight };
        SDL_Color borderColor =  ClientMenuUtils::clientMenuDisableTextColor();
        SDL_SetRenderDrawColor(this->menuTextureRenderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        SDL_RenderDrawRect(this->menuTextureRenderer, &outlineRect);
    }
}

void ClientMenuComboBoxTexture::menuTextureTextFieldHandlerMouseEvent(SDL_Event *mouseEvent) {
    if (this->menuComboBoxEnabled == false) {
        return;
    }
    
    if (this->menuTextureSelected == true) {
        this->menuComboBoxSelectedIndex = -1;
        int indexIterator = 0;
        
        std::list<ClientMenuComboBoxOptionTexture *>::iterator iterator = this->menuComboBoxOptionsList->begin();
        for (*iterator; iterator != this->menuComboBoxOptionsList->end(); ++iterator, ++indexIterator) {
            ClientMenuComboBoxOptionTexture *teamOption = *iterator;
            
            if (teamOption->menuTextureTextFieldEventInside(mouseEvent) == true) {
                this->menuComboBoxSelectedIndex = indexIterator;
                break;
            }
        }
    }
    
    ClientMenuTextFieldTexture::menuTextureTextFieldHandlerMouseEvent(mouseEvent);
}

std::string ClientMenuComboBoxTexture::menuComboBoxSelection() {
    if (this->menuComboBoxSelectedIndex >= 0) {
        std::list<std::string>::iterator iter = this->menuComboBoxTextList->begin();
        std::advance(iter, this->menuComboBoxSelectedIndex);
        return *iter;
    }

    return "";
}

int ClientMenuComboBoxTexture::menucomboBoxSelectionIndex() {
    return this->menuComboBoxSelectedIndex;
}

std::string ClientMenuComboBoxTexture::menuTextGetTextString() {
    if (this->menuComboBoxSelectedIndex >= 0) {
        std::list<std::string>::iterator iter = this->menuComboBoxTextList->begin();
        std::advance(iter, this->menuComboBoxSelectedIndex);
        return *iter;
    }
    
    return ClientMenuTextTexture::menuTextGetTextString();
}

SDL_Color ClientMenuComboBoxTexture::menuTextureGetTextColor() {
    if (this->menuComboBoxEnabled == false) {
        return ClientMenuUtils::clientMenuDisableTextColor();
    }
    
    return ClientMenuTextTexture::menuTextureGetTextColor();
}

bool ClientMenuComboBoxTexture::menuComboBoxGetEnabled() {
    return this->menuComboBoxEnabled;
}