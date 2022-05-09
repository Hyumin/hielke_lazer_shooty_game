#include "SoundEffect.h"
#include <SDL.h>

SoundEffect::SoundEffect()
{
}

SoundEffect::~SoundEffect()
{

	Mix_FreeChunk(m_effect);
	m_effect = nullptr;
}

bool SoundEffect::LoadEffect(const std::string& _filePath)
{
	
	m_effect = Mix_LoadWAV(_filePath.c_str());
	if (m_effect == NULL)
	{
		printf("Could not load: %s \n", _filePath);
		return false;
	}


	return true;
}

void SoundEffect::PlayEffect()
{
	if (m_effect != nullptr)
	{
		//m_effect->
		Mix_Volume(-1, 20);
		Mix_PlayChannel( - 1, m_effect, 0);
	}
}
