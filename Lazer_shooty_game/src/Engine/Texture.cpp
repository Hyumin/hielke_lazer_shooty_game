#include "Texture.h"

Texture::Texture()
{
	m_Width = 0;
	m_Height = 0;
	m_Name = "\0";
	m_Texture = NULL;
}

Texture::~Texture()
{
	SDL_DestroyTexture(m_Texture);
	m_Texture = NULL;
}

bool Texture::LoadTexture(std::string _path, SDL_Renderer* _renderer)
{

	bool succes = true;

	if (m_Texture != NULL)
	{
		//If we're trying to load a new texture on top of this texture for some reason
		SDL_DestroyTexture(m_Texture);
		m_Texture = NULL;
	}


	SDL_Surface* loadedSurface = IMG_Load(_path.c_str());

	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image error: %s\n", _path.c_str(), IMG_GetError());
		succes = false;
	}
	else
	{
		//Create textture from surface pixel
		m_Texture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
		if (m_Texture == NULL)
		{
			printf("Unable to create from %s! SDL Error: %s \n", _path.c_str(), SDL_GetError());
			succes = false;
		}
		//Get width and height from the surface before we delete it
		m_Width = loadedSurface->w;
		m_Height =loadedSurface->h;
		//Assign name so its equal to the path
		m_Name = _path;

		SDL_FreeSurface(loadedSurface);
	}

	return succes;
}
