#include "SoundManager.h"

SoundManager* SoundManager::s_pInstance;

SoundManager::SoundManager()
{
    Mix_OpenAudio(22050,AUDIO_S16, 2, (4096 / 2));
}

SoundManager::~SoundManager()
{
    Mix_CloseAudio();
}

bool SoundManager::load(std::string fileName, int id, sound_type type)
{
    if(type == SOUND_MUSIC)
    {
        Mix_Music* pMusic = Mix_LoadMUS(fileName.c_str());
        if(pMusic == 0)
        {
            std::cout << "Could not load music: ERROR - " << Mix_GetError() << std::endl;
            return false;
        }
        
        m_music[id] = pMusic;
        return true;
    }
    else if(type == SOUND_SFX)
    {
        Mix_Chunk* pChunk = Mix_LoadWAV(fileName.c_str());
        if(pChunk == 0)
        {
            std::cout << "Could not load SFX: ERROR - " << Mix_GetError() << std::endl;
            return false;
        }
        
        m_sfxs[id] = pChunk;
        return true;
    }
    return false;
}
void SoundManager::loadAudio(){
	load("Assets/Audio/Music/Master Kung-Fu - Good Chemistry.mp3",1,SOUND_MUSIC);
	load("Assets/Audio/Music/Fight Till Dying.mp3",2,SOUND_MUSIC);
	load("Assets/Audio/Music/Network Music Ensemble (Gina Brigida) - The Heart of a Champion.mp3",3,SOUND_MUSIC);
	load("Assets/Audio/Effects/IMPACT_Energy_Solid_12_mono.wav",51,SOUND_SFX);
	load("Assets/Audio/Effects/IMPACT_Bullet_Metal_06_mono.wav",52,SOUND_SFX);
	load("Assets/Audio/Effects/EXPLOSION_Short_Messy_stereo.wav",53,SOUND_SFX);
	load("Assets/Audio/Effects/Machine Gun Bursts 05.wav",54,SOUND_SFX);

}
void SoundManager::playMusic(int id, int loop)
{
    Mix_PlayMusic(m_music[id], loop);
}

void SoundManager::playSound(int id, int loop)
{
    Mix_PlayChannel(-1, m_sfxs[id], loop);
}
