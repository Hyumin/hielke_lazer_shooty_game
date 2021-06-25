#pragma once
#include <vector>
#include <string>
#include <SDL.h>
#include "..\HielkMath.h"
#include <SDL_ttf.h>

class SDLRenderer;
class Object;
class Enemy;
class Vector2;
class FloatingText;
class ResourceManager;

namespace Hielke
{

	class PlayerCharacter;
	//Struct that has a box collider and mapname use this to transittion to the map of mapname
	struct ConnectedMap
	{
		std::string mapName;
		std::string fileName;
		Box collider;
		Vector2 startPos;
	};


	struct HitResult
	{
		std::vector<PlayerCharacter*> players;
		std::vector<Enemy*> enemies;
	};

	class Map
	{
	public:
		Map();
		~Map();
		void Update(float _dt);
		void Render(SDLRenderer* _renderer, Vector2 _worldPos);
		void RenderZoomed(SDLRenderer* _renderer, Vector2 _worldPos, float _zoom);

		void SaveMap(const std::string& _filepath);//Saves current map to filepath
		void LoadMap(const std::string& _filePath, ResourceManager* _res);
		void LoadCollidersFromFile(const std::string& _filePath, const Object* _obj);
		Object* GetBackground();
		ConnectedMap& CheckPlayerCollisionWithConnectedMap(Box& _playerCollider);
		HitResult CheckMapCollision(Box& _collider);

		void SetBackGround(Object* _background);
		void AssignPlayer(PlayerCharacter* _player);
		void AssignEnemy(Enemy* _enem);
		void AddFloatingText(Vector2 _pos, SDL_Color _col, float _duration,float _speed, const std::string& _text);
		void SetDefaultFont(TTF_Font* _defaultFont);
		void AddWall(Box _box);

		std::string& GetMapName()
		{
			return m_MapName;
		}
		std::vector<Box>& GetColliders()
		{
			return m_Walls;
		}

		bool m_DebugMode;

		std::vector<ConnectedMap>& GetConnectedMaps()
		{
			return m_ConnectedMaps;
		}
		const std::vector<Enemy*>& GetEnemies()
		{
			return m_Enemies;
		}

	private:
		std::string m_MapName, m_VisalName, m_ColliderMapName;

		Object* m_BackGround;
		std::vector<Box> m_Walls;//The typical collider that just blocks movement
		std::vector<ConnectedMap> m_ConnectedMaps;
		std::vector<PlayerCharacter*> m_Players;
		std::vector<Enemy*> m_Enemies;
		std::vector<FloatingText*> m_FloatingTexts;
		TTF_Font* m_DefaultFont;
	};

};