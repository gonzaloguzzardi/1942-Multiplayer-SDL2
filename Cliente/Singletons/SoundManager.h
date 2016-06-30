#ifndef SINGLETONS_SOUNDMANAGE_H
#define SINGLETONS_SOUNDMANAGE_H

#include <iostream>
#include <map>
#include <string>

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>

enum sound_type
{
    SOUND_MUSIC = 0,
    SOUND_SFX = 1
};

class SoundManager
{
public:
    
    static SoundManager* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new SoundManager();
            return s_pInstance;
        }
        return s_pInstance;
    }
    
    bool load(std::string fileName, int id, sound_type type);
    
    void playSound(int id, int loop);
    void playMusic(int id, int loop);
    void loadAudio();
private:
    
    static SoundManager* s_pInstance;
    
    std::map<int, Mix_Chunk*> m_sfxs;
    std::map<int, Mix_Music*> m_music;
    
    SoundManager();
    ~SoundManager();

    SoundManager(const SoundManager&);
	SoundManager& operator=(const SoundManager&);
};

typedef SoundManager TheSoundManager;

#endif /* defined(SINGLETONS_SOUNDMANAGE_H) */
