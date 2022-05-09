#pragma once

#include <SDL_mixer.h>
#include <string>
class SoundEffect
{
public:
	SoundEffect();
	~SoundEffect();
	bool LoadEffect(const std::string& _filePath);

	void PlayEffect();

	Mix_Chunk* m_effect = NULL;
private:
};

