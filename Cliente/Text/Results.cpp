#include "Results.h"
using namespace std;

Results::Results(int gameWidth, int gameHeight, FinishGameInfo info)
{
	if (info.winnerID == -1)
	{
		generateDraw(gameWidth,gameHeight);
	}
    else
    {
    	if (info.isVictory)
    	{
			if(Game::Instance()->isTeamMode())
			{
				generateTeamWin(gameWidth, gameHeight, info.winnerID, info.points);
			}
			else
			{
				generateGeneralWin(gameWidth, gameHeight, info.winnerID, info.points);
			}
    	}
		else
		{
			generateLoss(gameWidth, gameHeight);
		}
    }
 }

Results::~Results()
{

}

void Results::generateTeamWin(int gameWidth, int gameHeight, short winnerID, short points)
{
	int h,w;

	FontTexture *title = new FontTexture();
	const char *pchar;
	stringstream ss;
	ss << "TEAM " << winnerID + 1 << " WINS";
	string s = ss.str();
	pchar = s.c_str();
    title->text = pchar;
    title->texture = FontManager::Instance()->drawtext(0,0,0,0,0,0,0,0,title->text,blended);
    FontManager::Instance()->textSize(title->text,&h,&w);
    title->height = h*TEXT_SIZE_FACTOR;
    title->width = w*TEXT_SIZE_FACTOR;
    title->x = (gameWidth-title->width)/2;
    title->y = (gameHeight-title->height)/2;
    m_texts.push_back(title);

    FontTexture *highScore = new FontTexture();
    createFontTexture(points,"POINTS: ",highScore);
    FontManager::Instance()->textSize(highScore->text,&h,&w);
    highScore->height = h*TEXT_SIZE_FACTOR;
    highScore->width = w*TEXT_SIZE_FACTOR;
    highScore->x = (gameWidth-highScore->width)/2;
    highScore->y = title->y + title->height;
    m_texts.push_back(highScore);
}

void Results::generateGeneralWin(int gameWidth, int gameHeight, short winnerID, short points)
{
	int h,w;

	FontTexture *title = new FontTexture();
	title->text = "SUCCESS";
	title->texture = FontManager::Instance()->drawtext(0,0,0,0,0,0,0,0,title->text,blended);
	FontManager::Instance()->textSize(title->text,&h,&w);
	title->height = h*TEXT_SIZE_FACTOR;
	title->width = w*TEXT_SIZE_FACTOR;
	title->x = (gameWidth-title->width)/2;
	title->y = (gameHeight-title->height)/2;
	m_texts.push_back(title);

	FontTexture *hs = new FontTexture();
	hs->text = "HIGHSCORE";
	hs->texture = FontManager::Instance()->drawtext(0,0,0,0,0,0,0,0,hs->text,blended);
	FontManager::Instance()->textSize(hs->text,&h,&w);
	hs->height = h*TEXT_SIZE_FACTOR;
	hs->width = w*TEXT_SIZE_FACTOR;
	hs->x = (gameWidth-hs->width)/2;
	hs->y = title->y + 2* title->height;
	m_texts.push_back(hs);

	FontTexture *highScore = new FontTexture();
	const char *pchar;
	stringstream ss;
	ss << "PLAYER " << winnerID + 1 << ": "<< points;
	string s = ss.str();
	pchar = s.c_str();
	highScore->text = pchar;
	highScore->texture = FontManager::Instance()->drawtext(0,0,0,0,0,0,0,0,highScore->text,blended);
	FontManager::Instance()->textSize(highScore->text,&h,&w);
	highScore->height = h*TEXT_SIZE_FACTOR;
	highScore->width = w*TEXT_SIZE_FACTOR;
	highScore->x = (gameWidth-highScore->width)/2;
	highScore->y = hs->y + hs->height;
	m_texts.push_back(highScore);
}

void Results::generateLoss(int gameWidth, int gameHeight)
{
	int h,w;

	FontTexture *title = new FontTexture();
	title->text = "GAME OVER";
	title->texture = FontManager::Instance()->drawtext(0,0,0,0,0,0,0,0,title->text,blended);
	FontManager::Instance()->textSize(title->text,&h,&w);
	title->height = h*TEXT_SIZE_FACTOR;
	title->width = w*TEXT_SIZE_FACTOR;
	title->x = (gameWidth-title->width)/2;
	title->y = (gameHeight-title->height)/2;
	m_texts.push_back(title);

}

void Results::generateDraw(int gameWidth, int gameHeight)
{
 	int h,w;

 	FontTexture *title = new FontTexture();
 	title->text = "DRAW";
 	title->texture = FontManager::Instance()->drawtext(0,0,0,0,0,0,0,0,title->text,blended);
 	FontManager::Instance()->textSize(title->text,&h,&w);
 	title->height = h*TEXT_SIZE_FACTOR;
 	title->width = w*TEXT_SIZE_FACTOR;
 	title->x = (gameWidth-title->width)/2;
 	title->y = (gameHeight-title->height)/2;
 	m_texts.push_back(title);

}

void Results::createFontTexture(short value, const char* text, FontTexture* font)
{
    stringstream ss;
    ss << text << value;
    string s = ss.str();
    const char *pchar = s.c_str();
    font->text = pchar;
    font->texture = FontManager::Instance()->drawtext(0,0,0,0,0,0,0,0, font->text, blended);
}
