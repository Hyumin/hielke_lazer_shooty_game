#include "ResourceManager.h"
#include <iostream>
#include "AnimationClip.h"


const char* DefaultTextureName = "Assets\\DefaultImage.png";

ResourceManager::ResourceManager()
{
	m_RenderEngine = nullptr;
	m_Textures.clear();
}

ResourceManager::ResourceManager(SDLRenderer* _EnginePointer)
{
	m_RenderEngine = _EnginePointer;
	m_Textures.clear();
	LoadDefaultMedia();
}


ResourceManager::~ResourceManager()
{
	//Dereference engine no need to delete that should happen in main
	m_RenderEngine = nullptr;
	m_Textures.clear();
	m_Maps.clear();
	m_Fonts.clear();
}

void ResourceManager::SetEngine(SDLRenderer* _EnginePointer)
{
	m_RenderEngine = _EnginePointer;
}

void ResourceManager::LoadDefaultMedia()
{

	LoadTexture(DefaultTextureName);
	LoadFont("Assets//Fonts//Jupiter.ttf",32);
	LoadFont("Assets//Fonts//arial.ttf",32);
	LoadFont("Assets//Fonts//LEELAWDB.TTF",12);


}

Texture* ResourceManager::GetTexture(const std::string& _texName)
{

	if (m_Textures[_texName] != NULL)
	{
		return m_Textures[_texName];
	}
	else
	{
		printf("Resrouce Manager: failed to get texture from map \n");
		//This will add m_textoload to the map if succesfull
		Texture* textoload = LoadTexture(_texName);
		if (textoload == NULL)
		{
			printf("Resource Manager :Failed to load texutre returning default instead \n");
			return m_Textures[std::string(DefaultTextureName)];
		}
		return textoload;
	}

	return nullptr;
}

Texture* ResourceManager::LoadTexture(std::string _path)
{

	Texture* textoload = new Texture();


	if (m_RenderEngine != nullptr)
	{

		//if its succesfull add to map and return the texture
		if (textoload->LoadTexture(_path, m_RenderEngine->GetRenderer()))
		{
			m_Textures[_path] = textoload;
			return textoload;
		}
		else
		{
			delete textoload;
			textoload = NULL;
		}
	}
	else
	{
		printf("Resource Manager: Render engine pointer was a nullpointer it needs to be defined for loadtexture to work \n");
	}


	return nullptr;
}

TTF_Font* ResourceManager::GetFont(const std::string& _fontName)
{

	if (m_Fonts[_fontName] != nullptr)
	{
		return m_Fonts[_fontName];
	}
	else
	{
		//attempt to load
		return LoadFont(_fontName,32);
	}
}

TTF_Font* ResourceManager::LoadFont(const std::string& _fontPath,int _size)
{

	if (m_Fonts[_fontPath] != nullptr)
	{
		return m_Fonts[_fontPath];
	}
	else
	{
		TTF_Font* font = TTF_OpenFont(_fontPath.data(), _size);

		if (font != nullptr)
		{
			m_Fonts[_fontPath] = font;
			return font;
		}
		else
		{
			return nullptr;
		}
	}
}

Hielke::Map* ResourceManager::GetMap(const std::string& _fileName)
{
	Hielke::Map* map = m_Maps[_fileName];
	if (map == nullptr)
	{
#ifdef DEBUG
		std::cout << " could not find file name " << fileName << std::endl;
#else
#endif
		map = LoadMap(_fileName);
	}

	return map;
}

Hielke::Map* ResourceManager::LoadMap(const std::string& _fileName)
{
	if (m_Maps[_fileName])
	{
#ifdef DEBUG
		std::cout << "Filename: " << _fileName << " already exists returning existing one \n";
#endif // DEBUG
		return m_Maps[_fileName];
	}

	Hielke::Map* newMap = new Hielke::Map();
	try
	{
		newMap->LoadMap(_fileName,this);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return newMap;
}


