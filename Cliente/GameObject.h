/*
 * GameObject.h
 *
 *  Created on: Apr 8, 2016
 *      Author: gonzalo
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <string>
#include "Vector2D.h"
#include "Singletons/ObjectIdGenerator.h"
#include "Utils/TiposDefinidos.h"

/*
 * Clase abstracta de la que heredan todos los GameObjects
*/
class GameObject
{
public:

    // clase abstracta
    virtual ~GameObject() {}

    /* Debe ser llamado siempre al crear un GameObject, para inicializar sus variables, cargar la imagen en el map del TextureManager, etc
     * x e y son la posición en pantalla en donde se inicializa el GameObject
     * width y height son el ancho y alto del sprite correspondiente al GameObject
     * textureID es el identificador con el que se vincula el sprite en el map del TextureManager.
     * numFrames son la cantidad de Frames
    */
    virtual void load(int x, int y, int width, int height, int textureID, int numFrames){};

    // Dibuja el objeto en pantalla respecto del estado actual del objeto
    virtual void draw() {};

    // Actualiza el estado del objeto (posición, velocidad, etc)
    virtual void update() {};

    // Libera memoria y los recursos que esté utilizando el GameObject
    virtual void clean(){};


    // Getters
    const std::string getTag() { return m_tag; }

    Vector2D& getPosition() { return m_position; }

    int getWidth() { return m_width; }
    int getHeight() { return m_height; }

    int getTextureId() { return m_textureID; }
    int getObjectId() { return m_objectId; }
    int getLayer() { return m_layer; }
    void setObjectID(int objectId) {m_objectId = objectId;}
    void setLayer(int layer) {m_layer = layer;}
    void setTextureID(int textureID) {m_textureID = textureID;}
    void setAlpha (int alpha) { m_alpha = alpha; }
    void setAngle (int angle) { m_angle = angle; }


    // En este ejemplo de scroll, scrollea to do menos los objetos con tag player
   /* void scroll(float scrollSpeed)
    {
        if(m_tag != std::string("Player")) // player is never scrolled
        {
            m_position.setX(m_position.getY() + scrollSpeed);
        }
    }*/

protected:

    // constructor with default initialisation list
    GameObject() :  m_tag("GameObject"),
					m_objectId(100),
    				m_textureID(0),
					m_layer(MIDDLEGROUND),
    				m_position(0,0),
                    m_width(0),
                    m_height(0),
					m_numFrames(0),
                    m_angle(0),
                    m_alpha(255),
                    m_currentRow(0),
                    m_currentFrame(0)


    {
    }

    // Tag que posee información acerca del tipo de GameObject
    std::string m_tag;
    int m_objectId;
    int m_textureID;
    int m_layer;
    // posición del objeto en el juego.
    Vector2D m_position;

    // Variables del sprite/spritesheet del GameObject en cuestión
    //Ancho del sprite
    int m_width;
    //Alto del sprite
    int m_height;
    //ID del sprite que tomará en el map de lTextureManager

    //numero de frames del sprite en el archivo de imagen
    int m_numFrames;

    // ángulo de rotación en sentido horario
    double m_angle;
    // transparencia: 255 visible - 0 invisible
    int m_alpha;

    // variables para controlar la animación
    int m_currentRow;
    //actual frame que se deberá dibujar
    int m_currentFrame;

};


#endif /* GAMEOBJECT_H_ */
