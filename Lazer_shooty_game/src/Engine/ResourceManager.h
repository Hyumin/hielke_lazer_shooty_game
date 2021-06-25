#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <string>
#include "SDLRenderer.h"
#include "Texture.h"
#include "Map.h"

class ResourceManager
{

public:
	ResourceManager();
	ResourceManager(SDLRenderer* _EnginePointer);
	~ResourceManager();

	void SetEngine(SDLRenderer* _EnginePointer);
	void LoadDefaultMedia();

	Texture* GetTexture(const std::string& _texName );//Will try to get a texture with the same name as the input string, if failed will try to load it if that fails to returns nullptr
	Texture* LoadTexture(std::string _path ); //Will load a texture add it to the map and return the pointer to the newly loaded texture if succesfull
	
	TTF_Font* GetFont(const std::string& _fontName);
	TTF_Font* LoadFont(const std::string& _fontPath, int _size);

	Hielke::Map* GetMap(const std::string& _fileName);
	Hielke::Map* LoadMap(const std::string& _fileName);



private:
	std::map<std::string, TTF_Font*>m_Fonts;
	std::map<std::string, Texture*> m_Textures;
	std::map<std::string, Hielke::Map*> m_Maps;


	SDLRenderer* m_RenderEngine;
};

