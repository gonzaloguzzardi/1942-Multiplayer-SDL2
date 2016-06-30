//
//  ClientMenuComboBoxTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/15/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuComboBoxTexture_hpp
#define ClientMenuComboBoxTexture_hpp

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <list>

#include "ClientMenuComboBoxOptionTexture.hpp"
#include "ClientMenuTextFieldTexture.hpp"

class ClientMenuComboBoxTexture: public ClientMenuTextFieldTexture {
public:
    ClientMenuComboBoxTexture(SDL_Window *menuWindow, std::string textureText, std::list<std::string> *optionsList);
    virtual ~ClientMenuComboBoxTexture();
    
    virtual void menuTextureRender(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    
    virtual std::string menuTextGetTextString();
    virtual SDL_Color menuTextureGetTextColor();
    bool menuComboBoxGetEnabled();
    
    // Events handlers.
    virtual void menuTextureTextFieldHandlerMouseEvent(SDL_Event *mouseEvent);
    
    std::string menuComboBoxSelection();
    int menucomboBoxSelectionIndex();
    
private:
    SDL_Window *menuComboBoxWindow;
    int menuComboOptionsFontSize;
    
    bool menuComboBoxEnabled;
    
    int menuComboBoxSelectedIndex;
    
    std::list<std::string> *menuComboBoxTextList;
    std::list<ClientMenuComboBoxOptionTexture *> *menuComboBoxOptionsList;
    
    void menuComboBoxRenderOptions();
};

#endif /* ClientMenuComboBoxTexture_hpp */
