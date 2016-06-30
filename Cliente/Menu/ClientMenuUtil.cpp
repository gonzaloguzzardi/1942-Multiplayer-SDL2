//
//  ClientMenuUtil.cpp
//  Menu
//
//  Created by Gaston Montes on 6/14/16.
//  Copyright © 2016 Gaston Montes. All rights reserved.
//

#include "ClientMenuUtil.hpp"

SDL_Color ClientMenuUtils::clientMenuModulationColor() {
    return { 140, 140, 140 };
}

SDL_Color ClientMenuUtils::clientMenuTextColor() {
    return { 98, 174, 50 };
}

SDL_Color ClientMenuUtils::clientMenuTextColorSelected() {
    return { 255, 255, 255 };
}

SDL_Color ClientMenuUtils::clientMenuDisableTextColor() {
    return { 140, 140, 140 };
}

std::list<std::string> ClientMenuUtils::clientMenuAllowedCharacters() {
    std::list<std::string> allowedCharacters = std::list<std::string>();
    allowedCharacters.push_back("a");
    allowedCharacters.push_back("b");
    allowedCharacters.push_back("c");
    allowedCharacters.push_back("d");
    allowedCharacters.push_back("e");
    allowedCharacters.push_back("f");
    allowedCharacters.push_back("g");
    allowedCharacters.push_back("h");
    allowedCharacters.push_back("i");
    allowedCharacters.push_back("j");
    allowedCharacters.push_back("k");
    allowedCharacters.push_back("l");
    allowedCharacters.push_back("m");
    allowedCharacters.push_back("n");
    allowedCharacters.push_back("o");
    allowedCharacters.push_back("p");
    allowedCharacters.push_back("q");
    allowedCharacters.push_back("r");
    allowedCharacters.push_back("s");
    allowedCharacters.push_back("t");
    allowedCharacters.push_back("u");
    allowedCharacters.push_back("v");
    allowedCharacters.push_back("w");
    allowedCharacters.push_back("x");
    allowedCharacters.push_back("y");
    allowedCharacters.push_back("z");
    allowedCharacters.push_back("A");
    allowedCharacters.push_back("B");
    allowedCharacters.push_back("C");
    allowedCharacters.push_back("D");
    allowedCharacters.push_back("E");
    allowedCharacters.push_back("F");
    allowedCharacters.push_back("G");
    allowedCharacters.push_back("H");
    allowedCharacters.push_back("I");
    allowedCharacters.push_back("J");
    allowedCharacters.push_back("K");
    allowedCharacters.push_back("L");
    allowedCharacters.push_back("M");
    allowedCharacters.push_back("N");
    allowedCharacters.push_back("O");
    allowedCharacters.push_back("P");
    allowedCharacters.push_back("Q");
    allowedCharacters.push_back("R");
    allowedCharacters.push_back("S");
    allowedCharacters.push_back("T");
    allowedCharacters.push_back("U");
    allowedCharacters.push_back("V");
    allowedCharacters.push_back("W");
    allowedCharacters.push_back("X");
    allowedCharacters.push_back("Y");
    allowedCharacters.push_back("Z");
    allowedCharacters.push_back("0");
    allowedCharacters.push_back("1");
    allowedCharacters.push_back("2");
    allowedCharacters.push_back("3");
    allowedCharacters.push_back("4");
    allowedCharacters.push_back("5");
    allowedCharacters.push_back("6");
    allowedCharacters.push_back("7");
    allowedCharacters.push_back("8");
    allowedCharacters.push_back("9");
    allowedCharacters.push_back(" ");
    
    return allowedCharacters;
}

TTF_Font *ClientMenuUtils::clientMenuFont(int fontSize) {
    return TTF_OpenFont("Assets/Fonts/bummer3d.ttf", fontSize);
}
