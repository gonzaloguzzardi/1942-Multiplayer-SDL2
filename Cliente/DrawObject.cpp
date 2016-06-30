#include "DrawObject.h"
using namespace std;

DrawObject::DrawObject() : GameObject() {
	// TODO Auto-generated constructor stub

}

DrawObject::~DrawObject() {
	// TODO Auto-generated destructor stub
}

void DrawObject::load(int x, int y, int width, int height, int textureID, int numFrames)
{
    // Load comun. Inicializa variables
	m_position = Vector2D(x, y);

	// Informacion del sprite/spritesheet
	TextureInfo data = TextureManager::Instance()->getTextureInfo(textureID);
	m_width = data.width;
	m_height = data.height;
	m_textureID = textureID;
    m_numFrames = data.numFrames;

    TextureManager::Instance()->load(data.path, m_textureID, Game::Instance()->getRenderer());


}


void DrawObject::load(int x, int y, int textureID)
{

    // Load comun. Inicializa variables
	m_position = Vector2D(x, y);

	// Informacion del sprite/spritesheet
	TextureInfo data = TextureManager::Instance()->getTextureInfo(textureID);
	m_width = data.width;
	m_height = data.height;
	m_textureID = textureID;
    m_numFrames = data.numFrames;

    TextureManager::Instance()->load(std::string(data.path), m_textureID, Game::Instance()->getRenderer());


}

void DrawObject::draw()
{

    TextureManager::Instance()->drawFrame(m_textureID, m_position.getX(), m_position.getY(), m_width, m_height,
    										m_currentRow, m_currentFrame, Game::Instance()->getRenderer(), m_angle, m_alpha, SDL_FLIP_NONE);
	//Nave::draw();
}



void DrawObject::clean()
{

}

