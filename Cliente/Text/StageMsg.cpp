#include "StageMsg.h"
using namespace std;

StageMsg::StageMsg(int gameWidth, int gameHeight, StageBeginning info)
{
    int h,w;
    FontTexture *msg;

    msg = new FontTexture();
    createFontTexture(info.stageNumber,"Beginning Stage Number ",msg);
    FontManager::Instance()->textSize(msg->text,&h,&w);
    msg->height = h*TEXT_SIZE_FACTOR;
    msg->width = w*TEXT_SIZE_FACTOR;
    msg->x = (gameWidth-msg->width)/2;
    msg->y = 3*gameHeight/4;
    m_texts.push_back(msg);
}

StageMsg::~StageMsg()
{

}

void StageMsg::createFontTexture(short value, const char* text, FontTexture* font)
{
    stringstream ss;
    ss << text << value;
    string s = ss.str();
    const char *pchar = s.c_str();
    font->text = pchar;
    font->texture = FontManager::Instance()->drawtext(190,100,25,0,0,0,0,0, font->text, blended);
}
