//
//  ClientMenuComboBoxOptionTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/15/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuComboBoxOptionTexture_hpp
#define ClientMenuComboBoxOptionTexture_hpp

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <stdio.h>

#include "ClientMenuTextFieldTexture.hpp"

class ClientMenuComboBoxOptionTexture : public ClientMenuTextFieldTexture {
public:
    ClientMenuComboBoxOptionTexture(SDL_Window *menuWindow, std::string textureText);
    virtual ~ClientMenuComboBoxOptionTexture();
    
    virtual void menuTextureRender(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

#endif /* ClientMenuComboBoxOptionTexture_hpp */
