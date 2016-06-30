//
//  ClientMenuImageTexture.hpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClientMenuImageTexture_hpp
#define ClientMenuImageTexture_hpp

#include <stdio.h>

#include "ClientMenuTexture.hpp"

class ClientMenuImageTexture : public ClientMenuTexture {
public:
    ClientMenuImageTexture(SDL_Window *menuWindow);
    virtual ~ClientMenuImageTexture();
    
    //Loads image at specified path
    void menuTextureLoadFromFile(const char *filePath);
};

#endif /* ClientMenuImageTexture_hpp */
