//
//  ClientMenuTextFieldTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/15/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuTextFieldTexture_hpp
#define ClientMenuTextFieldTexture_hpp

#include <stdio.h>
#include <string>
#include <time.h>

#include "ClientMenuTextTexture.hpp"

class ClientMenuTextFieldTexture: public ClientMenuTextTexture {
public:
    ClientMenuTextFieldTexture(SDL_Window *menuWindow, std::string textureText);
    virtual ~ClientMenuTextFieldTexture();
    
    virtual void menuTextureRender(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    
    // Events handlers.
    virtual void menuTextureTextFieldHandlerMouseEvent(SDL_Event *mouseEvent);
    void menuTextureTextFieldHanldlerInputEvent(SDL_Event *inputEvent);
    
    bool menuTextureTextFieldIsSelected();
    
    bool menuTextureTextFieldEventInside(SDL_Event *mouseEvent);
    
protected:
    clock_t menuTextureTextFieldStartTime;
    bool menuTextureTextFieldDrawLine;
    
private:
    bool menuTextureTextFieldInputIsBackSpace(SDL_Event *inputEvent);
    bool menuTextureTextFieldInputIsReturn(SDL_Event *inputEvent);
};

#endif /* ClientMenuTextFieldTexture_hpp */
