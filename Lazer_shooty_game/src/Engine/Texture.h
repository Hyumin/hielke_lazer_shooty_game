#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool LoadTexture(std::string _path,SDL_Renderer* _renderer);
	unsigned int GetWidth() { return m_Width; }
	unsigned int GetHeight() { return m_Height; }
	std::string GetName() { return m_Name; }
	SDL_Texture* GetTexture() { return m_Texture; }


private:

	unsigned int m_Width,m_Height;
	std::string m_Name;

	SDL_Texture* m_Texture;

};

