//
//  ClienteMenu.hpp
//  Menu
//
//  Created by Gaston Montes on 6/13/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#ifndef ClienteMenu_hpp
#define ClienteMenu_hpp

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>

#include "ClientMenuButtonTexture.hpp"
#include "../Utils/TiposDefinidos.h"

class ClientMenu: public ClientMenuButtonTextureDelegate {
public:
    // Constructor and Destructor.
    ClientMenu(const char *menuTitle, const int menuWidth, const int menuHeight, std::vector<GameTeam> teamOptionsList);
    virtual ~ClientMenu();
    
    // Main function.
    void clientMenuRun();
    
    // Delegate functions.
    virtual void menuButtonTouched();
    
    // Getters.
    std::string clientMenuGetPlayerName();
    GameTeam clientMenuGetPlayerTeam();
    bool clienMenuGetPlayButtonSelected();
    
private:
    SDL_Window *clientMenuWindow;
    SDL_Renderer *clientMenuRender;
    
    std::string clientMenuPlayerNameSelected;
    GameTeam clientMenuPlayerTeamSelected;
    
    std::vector<GameTeam> clientMenuPlayerTeamOptionsList;
    
    bool clientMenuPlayButtonSelected;
    
    // Drawing methods.
    bool clientMenuLoadTexture(const char *imageName);

    // Showing functions.
    void clientMenuClear();
    void clientMenuShow();
    
    // Events methods.
    bool clientMenuHandleQuitEvent(SDL_Event *event);
};

#endif /* ClienteMenu_hpp */
