//
//  ClientMenuButtonTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/15/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuButtonTexture_hpp
#define ClientMenuButtonTexture_hpp

#include <SDL2/SDL.h>
#include <stdio.h>

#include "ClientMenuTextFieldTexture.hpp"

class ClientMenuButtonTextureDelegate {
public:
    ClientMenuButtonTextureDelegate(){}
    virtual ~ClientMenuButtonTextureDelegate(){}
    
    virtual void menuButtonTouched()=0;
};

class ClientMenuButtonTexture: public ClientMenuTextFieldTexture {
public:
    ClientMenuButtonTexture(SDL_Window *menuWindow, std::string textureText, ClientMenuButtonTextureDelegate *buttonDelegate);
    virtual ~ClientMenuButtonTexture();
    
    virtual void menuTextureRender(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    virtual void menuTextureTextFieldHandlerMouseEvent(SDL_Event *mouseEvent);
    virtual SDL_Color menuTextureGetTextColor();
    
    void menuButtonSetEnabled(bool enabled);
    
private:
    bool menuButtonEnabled;
    
    ClientMenuButtonTextureDelegate *menuButtonDelegate;
};

#endif /* ClientMenuButtonTexture_hpp */
