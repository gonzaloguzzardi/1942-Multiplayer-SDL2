//
//  ClientMenuTextTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuTextTexture_hpp
#define ClientMenuTextTexture_hpp

#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>

#include "ClientMenuTexture.hpp"

class ClientMenuTextTexture : public ClientMenuTexture {
public:
    ClientMenuTextTexture(SDL_Window *menuWindow, std::string textureText);
    virtual ~ClientMenuTextTexture();
    
    void menuTextureSetTextFont(int fontSize);
    void menuTextureCreateTextSurface();
    
    virtual SDL_Color menuTextureGetTextColor();
    virtual TTF_Font *menuTextureGetTextFont(int fontSize);
    virtual std::string menuTextGetTextString();
    
protected:
    bool menuTextureSelected;
    
    std::string menuTextureText;
    
private:
    TTF_Font *menuTextureFont;
};

#endif /* ClientMenuTextTexture_hpp */
