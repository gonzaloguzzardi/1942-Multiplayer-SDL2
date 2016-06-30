/*
 * TextureManager.h
 *
 *  Created on: Apr 8, 2016
 *      Author: gonzalo
 */

#ifndef SINGLETONS_TEXTUREMANAGER_H_
#define SINGLETONS_TEXTUREMANAGER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Utils/TiposDefinidos.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>


#define DEFAULT_PATH "Assets/Sprites/missing.jpg"

class TextureManager
{
public:

    static TextureManager* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new TextureManager();
            return s_pInstance;
        }
        return s_pInstance;
    }

    //Carga en el map la textura del archivo fileName
    //devuelve true si se cargo con éxito y false si no lo hizo
    bool load(std::string fileName, int id, SDL_Renderer* pRenderer);

    //deprecated
    void init(SDL_Renderer* pRenderer);

    TextureInfo getTextureInfo(int textureId);
    void loadTextures(SDL_Renderer* pRenderer);
    void addTextureInfo(TextureInfo textureInfo);
    void removeTextureInfoAt(int textureID);

    void changeTextureColor(int textureID, Uint8 r, Uint8 g, Uint8 b);

    // Libera la memoria utilizada por todas las imagenes cargadas al map
    void clearTextureMap();
    // Libera la memoria de la imagen con id pasado como parametro (la saca del map)
    void clearFromTextureMap(int id);
    //Funcion para dibujar el objeto en pantalla en forma simple
    void draw(int id, int x, int y, int width, int height, double angle, SDL_Renderer* renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    //Funcion para dibujar el objeto en pantalla mas completa, tomando en cuenta las variables de animación y transparencia
    void drawFrame(int id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* renderer, double angle, int alpha, SDL_RendererFlip flip = SDL_FLIP_NONE);

    std::map<int, SDL_Texture*> getTextureMap() { return m_textureMap; }

private:

    std::map<int, SDL_Texture*> m_textureMap;
    std::map<int, TextureInfo> m_textureMapInfo;
    static TextureManager* s_pInstance;

    bool validPath(const std::string& filePath);

    TextureManager() {}
    ~TextureManager() {}
    TextureManager(const TextureManager&);
	TextureManager& operator=(const TextureManager&);
};

#endif /* SINGLETONS_TEXTUREMANAGER_H_ */
