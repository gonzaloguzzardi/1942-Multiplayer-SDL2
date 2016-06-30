//
//  ClientMenuTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuTexture_hpp
#define ClientMenuTexture_hpp

#include <SDL2/SDL.h>
#include <stdio.h>

class ClientMenuTexture {
public:
    ClientMenuTexture(SDL_Window *menuWindow);
    virtual ~ClientMenuTexture();
    
    //Renders texture at given point.
    virtual void menuTextureRender(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    
    //Gets image dimensions.
    int menuTextureGetWidth();
    int menuTextureGetHeight();
    
    bool menuTextureGetLoaded();
    
    SDL_Texture *menuTextureGetTexture();
    
    void menuTextureSetModulation(SDL_Color modulationColor);
    
protected:
    //Image dimensions.
    int menuTextureX;
    int menuTextureY;
    int menuTextureWidth;
    int menuTextureHeight;
    
    bool menuTextureLoaded;
    
    //The actual hardware texture.
    SDL_Texture *menuTexture;
    SDL_Renderer *menuTextureRenderer;
};

#endif /* ClientMenuTexture_hpp */
