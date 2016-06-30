#include "Lives.h"
using namespace std;

Lives::~Lives(){

}

Lives::Lives(int gameWidth, int gameHeight, int cantLives):
										 m_initialLives(cantLives),
										 m_playerLives(cantLives),
										 m_newLives(cantLives)
{
	int h,w;
	updateTexture(m_playerLives);
	FontManager::Instance()->textSize(m_lives.text,&h,&w);
	m_lives.height = h*TEXT_SIZE_FACTOR;
	m_lives.width = w*TEXT_SIZE_FACTOR;
	m_lives.x = 0;
	m_lives.y = gameHeight - m_lives.height;

	m_texts.push_back(&m_lives);
}

void Lives::actualizarLives(int lives)
{
	m_newLives = lives;
}


void Lives::update()
{
	if (m_newLives != m_playerLives)
	{
		m_playerLives = m_newLives;
		updateTexture(m_newLives);
	}
}

void Lives::reset()
{
	actualizarLives(m_initialLives);
}

void Lives::updateTexture(short value)
{
    stringstream ss;
    ss << "Lives: " << value;
    string s = ss.str();
    const char *pchar = s.c_str();
    m_lives.text = pchar;
    m_lives.texture = FontManager::Instance()->drawtext(255,255,255,0,0,0,0,0, m_lives.text, blended);
}
