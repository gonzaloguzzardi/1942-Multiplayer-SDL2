#include "Statistics.h"
using namespace std;

Statistics::Statistics(int gameWidth, int gameHeight, StageStatistics stats)
{
    int h,w;
    FontTexture* title,*points,*enemiesKilled,*accuracy;

    title = new FontTexture();
    title->text = "STATS";
    title->texture = FontManager::Instance()->drawtext(145,17,17,0,0,0,0,0,title->text,blended);
    FontManager::Instance()->textSize(title->text,&h,&w);
    title->height = h*TEXT_SIZE_FACTOR;
    title->width = w*TEXT_SIZE_FACTOR;
    title->x = (gameWidth-title->width)/2;
    title->y = 3*gameHeight/4;
    m_texts.push_back(title);
    
    points = new FontTexture();
    createFontTexture(stats.points,"Points: ",points);
    FontManager::Instance()->textSize(points->text,&h,&w);
    points->height = h*TEXT_SIZE_FACTOR;
    points->width = w*TEXT_SIZE_FACTOR;
    points->x = (gameWidth-points->width)/2;
    points->y = title->y + title->height;
    m_texts.push_back(points);
    
    enemiesKilled = new FontTexture();
    createFontTexture(stats.enemiesKilled,"Enemies Killed: ",enemiesKilled);
    FontManager::Instance()->textSize(enemiesKilled->text,&h,&w);
    enemiesKilled->height = h*TEXT_SIZE_FACTOR;
    enemiesKilled->width = w*TEXT_SIZE_FACTOR;
    enemiesKilled->x = (gameWidth-enemiesKilled->width)/2;
    enemiesKilled->y = points->y + points->height;
    m_texts.push_back(enemiesKilled);
    
    accuracy = new FontTexture();
    createFontTexture(stats.accuracy,"Accuracy: ",accuracy);
    FontManager::Instance()->textSize(accuracy->text,&h,&w);
    accuracy->height = h*TEXT_SIZE_FACTOR;
    accuracy->width = w*TEXT_SIZE_FACTOR;
    accuracy->x = (gameWidth-accuracy->width)/2;
    accuracy->y = enemiesKilled->y + enemiesKilled->height;
    m_texts.push_back(accuracy);
}

Statistics::~Statistics()
{

}

void Statistics::createFontTexture(short value, const char* text, FontTexture* font)
{
    stringstream ss;
    ss << text << value;
    string s = ss.str();
    const char *pchar = s.c_str();
    font->text = pchar;
    font->texture = FontManager::Instance()->drawtext(51,0,51,0,0,0,0,0, font->text, blended);
}
