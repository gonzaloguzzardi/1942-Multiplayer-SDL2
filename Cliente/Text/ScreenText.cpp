#include "ScreenText.h"
using namespace std;

ScreenText::~ScreenText(){

}

void ScreenText::draw(SDL_Renderer* renderer)
{
	SDL_Rect destRect;

	for(std::list<FontTexture*>::iterator it = m_texts.begin(); it != m_texts.end(); it ++)
	{
		destRect.w = (*it)->width;
		destRect.h = (*it)->height;
		destRect.x = (*it)->x;
		destRect.y = (*it)->y;
		SDL_RenderCopy(renderer, (*it)->texture, NULL, &destRect);
	}
}
