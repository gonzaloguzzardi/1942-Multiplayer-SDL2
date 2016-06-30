//
//  ClienteMenu.cpp
//  Menu
//
//  Created by Gaston Montes on 6/13/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include <string>

#include "ClientMenuTextFieldTexture.hpp"
#include "ClientMenuComboBoxTexture.hpp"
#include "ClientMenuImageTexture.hpp"
#include "ClientMenuTextTexture.hpp"
#include "ClientMenuTexture.hpp"
#include "ClientMenuUtil.hpp"
#include "ClienteMenu.hpp"

// Constructor and Destructor.
ClientMenu::ClientMenu(const char *menuTitle, const int menuWidth, const int menuHeight, std::vector<GameTeam> teamOptionsList) {
    this->clientMenuPlayerNameSelected = "";
    this->clientMenuPlayButtonSelected = false;
    this->clientMenuPlayerTeamOptionsList = teamOptionsList;
    
    // The window we'll be rendering to.
    this->clientMenuWindow = NULL;
    
    //Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("ClienteMenu.cpp - SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        //Create window.
        this->clientMenuWindow = SDL_CreateWindow(menuTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, menuWidth, menuHeight, SDL_WINDOW_SHOWN);
        
        if (this->clientMenuWindow == NULL) {
            printf("ClienteMenu.cpp - SDL Window could not be created! SDL_Error: %s\n", SDL_GetError());
        } else {
            //Create renderer for window
            this->clientMenuRender = SDL_CreateRenderer(this->clientMenuWindow, -1, SDL_RENDERER_SOFTWARE);
            if (this->clientMenuRender == NULL) {
                printf("ClienteMenu.cpp - Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            }
        }
    }
}

ClientMenu::~ClientMenu() {
    // Free screen surface.
    SDL_Surface *screenSurface = SDL_GetWindowSurface(this->clientMenuWindow);
    SDL_FreeSurface(screenSurface);
    
    //Destroy window
    SDL_DestroyRenderer(this->clientMenuRender);
    SDL_DestroyWindow(this->clientMenuWindow);
    
    //Quit SDL subsystems
    SDL_Quit();
}

// Render functions.
void ClientMenu::clientMenuClear() {
    // Clear screen.
    SDL_SetRenderDrawColor(this->clientMenuRender, 255, 255, 255, 255);
    SDL_RenderClear(this->clientMenuRender);
}

void ClientMenu::clientMenuShow() {
    // Update screen.
    SDL_RenderPresent(this->clientMenuRender);
}

// Events methods.
bool ClientMenu::clientMenuHandleQuitEvent(SDL_Event *event) {
    //Event handler
    while (SDL_PollEvent(event) != 0) {
        //User requests quit
        if (event->type == SDL_QUIT) {
            return true;
        }
    }
    
    return false;
}

// Delegate functions.
void ClientMenu::menuButtonTouched() {
    this->clientMenuPlayButtonSelected = true;
}

// Main function.
void ClientMenu::clientMenuRun() {
    ClientMenuImageTexture backgrounImage = ClientMenuImageTexture(this->clientMenuWindow);
    backgrounImage.menuTextureLoadFromFile("Assets/Sprites/Bomber.JPG");
    backgrounImage.menuTextureSetModulation(ClientMenuUtils::clientMenuModulationColor());
    
    ClientMenuTextTexture promptPlayerText = ClientMenuTextTexture(this->clientMenuWindow, "Nombre del jugador:");
    
    ClientMenuTextFieldTexture inputPlayerText = ClientMenuTextFieldTexture(this->clientMenuWindow, "Jugador #");
    
    std::list<std::string> teamNamesList;
    std::vector<GameTeam>::iterator iterator = this->clientMenuPlayerTeamOptionsList.begin();
    
    for (*iterator; iterator != this->clientMenuPlayerTeamOptionsList.end(); ++iterator) {
        GameTeam newGameTeam = *iterator;
        teamNamesList.push_back(newGameTeam.gameTeamName);
    }
    
    ClientMenuComboBoxTexture teamCombo = ClientMenuComboBoxTexture(this->clientMenuWindow, "Seleccione equipo", &teamNamesList);
    
    ClientMenuButtonTexture playButton = ClientMenuButtonTexture(this->clientMenuWindow, "Jugar", this);
    
    SDL_Event event;
    
    while (this->clientMenuHandleQuitEvent(&event) == false && this->clientMenuPlayButtonSelected == false) {
        this->clientMenuClear();
        
        // Handler mouse touch up event.
        if (event.type == SDL_MOUSEBUTTONUP) {
            inputPlayerText.menuTextureTextFieldHandlerMouseEvent(&event);
            teamCombo.menuTextureTextFieldHandlerMouseEvent(&event);
            playButton.menuTextureTextFieldHandlerMouseEvent(&event);
            while (SDL_PollEvent(&event) == 0) {}
        }
        
        if (inputPlayerText.menuTextureTextFieldIsSelected() == true && (event.type == SDL_KEYDOWN || event.type == SDL_TEXTINPUT)) {
            inputPlayerText.menuTextureTextFieldHanldlerInputEvent(&event);
            while (SDL_PollEvent(&event) == 0) {}
        }
        
        SDL_Rect imageRect;
        imageRect.x = 0;
        imageRect.y = 0;
        SDL_GetWindowSize(this->clientMenuWindow, &imageRect.w, &imageRect.h);
        
        // Background Image.
        backgrounImage.menuTextureRender(0, 0, &imageRect);
        
        // Player prompt text.
        promptPlayerText.menuTextureSetTextFont(imageRect.w / 20);
        promptPlayerText.menuTextureCreateTextSurface();
        promptPlayerText.menuTextureRender((imageRect.w - promptPlayerText.menuTextureGetWidth()) / 2,
                                           imageRect.h / 2 - promptPlayerText.menuTextureGetHeight() * 3);
        
        // Player input text.
        inputPlayerText.menuTextureSetTextFont(imageRect.w / 25);
        inputPlayerText.menuTextureCreateTextSurface();
        inputPlayerText.menuTextureRender((imageRect.w - inputPlayerText.menuTextureGetWidth()) / 2,
                                          imageRect.h / 2 - inputPlayerText.menuTextureGetHeight() * 2);
        
        // Team combo.
        teamCombo.menuTextureSetTextFont(imageRect.w / 20);
        teamCombo.menuTextureCreateTextSurface();
        teamCombo.menuTextureRender((imageRect.w - teamCombo.menuTextureGetWidth()) / 2,
                                    imageRect.h / 2 + teamCombo.menuTextureGetHeight());
        
        // Play Button.
        playButton.menuTextureSetTextFont(imageRect.w / 20);
        playButton.menuTextureCreateTextSurface();
        playButton.menuTextureRender((imageRect.w - playButton.menuTextureGetWidth()) / 2,
                                     imageRect.h - playButton.menuTextureGetHeight() * 2);
        
        if (inputPlayerText.menuTextGetTextString().length() > 2 && (teamCombo.menuComboBoxGetEnabled() == false || teamCombo.menuComboBoxSelection().length() > 0)) {
            this->clientMenuPlayerNameSelected = inputPlayerText.menuTextGetTextString();
            
            std::string selectedTeam = teamCombo.menuComboBoxSelection();
            int selectedTeamIndex = teamCombo.menucomboBoxSelectionIndex();
            
            if (selectedTeamIndex >= 0) {
                std::vector<GameTeam>::iterator iterator = this->clientMenuPlayerTeamOptionsList.begin();
                std::advance(iterator, selectedTeamIndex);
                this->clientMenuPlayerTeamSelected = *iterator;
            }
            
            playButton.menuButtonSetEnabled(true);
        } else {
            this->clientMenuPlayerNameSelected = "";
            
            playButton.menuButtonSetEnabled(false);
        }
        
        this->clientMenuShow();
    }
}

std::string ClientMenu::clientMenuGetPlayerName() {
    return this->clientMenuPlayerNameSelected;
}

GameTeam ClientMenu::clientMenuGetPlayerTeam() {
    return this->clientMenuPlayerTeamSelected;
}

bool ClientMenu::clienMenuGetPlayButtonSelected() {
    return this->clientMenuPlayButtonSelected;
}
