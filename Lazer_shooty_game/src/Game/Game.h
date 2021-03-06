#pragma once
#include "..\Engine\SDLRenderer.h"
#include "..\Engine\ResourceManager.h"
#include "..\Engine\Object.h"
#include "..\Engine\AnimationClip.h"
#include "..\Engine\TextField.h"
#include "..\Engine\FloatingText.h"
#include "..\Engine\Map.h"
#include "..\Debug\PlayerDebugInfo.h"
#include "Cannon.h"
#include "projectile.h"
#include "SpaceBackground .h"

struct Vec2
{
	float x, y;
};

class EnemySpawner;
class Game
{

public:
	Game();
	Game(ResourceManager* _resMan);
	~Game();


	void Init();

	void SetResourceManager(ResourceManager* _resMan);
	void Update(float _dt);
	void KeyDown(unsigned int _key);
	void KeyUp(unsigned int _key);
	void MouseMove(int _x, int _y);
	void MouseUp(int _key);
	void MouseDown(int _key);

	void Render(SDLRenderer* _renderer);
	void AddEnemey(Enemy* _enem);//Adds an enemy to the enemy array
	void UpdateEnemies(float _dt);
	
private:
	void SpawnBalls(int _num_balls);
	void ToggleDebugMode();

	ResourceManager* m_ResMan;
	Cannon* m_player_cannon;
	//Hielke::PlayerDebugInfo* m_DebugInfo;
	SoundEffect* m_LazerExplosion;

	Hielke::Map* m_CurrentMap;
	std::vector<FloatingText> m_FloatingTexts;

	bool m_kup, m_kdwn, m_klft, m_krght,m_ksht;
	Vector2 m_WorldPos;
	Vector2 m_WindowSize;
	Vector2 m_MousePos;

	float m_Speed = 100.0f;
	float m_AnimTimer = 0.0f;
	float m_AnimTimeLimit = 0.3f;

	bool m_DebugMode = false;// set this to true if you want see things like colliders etc.

	SDL_Keycode m_debug_mode_key = SDLK_1;
	std::vector<Enemy*> m_enemies;
	std::vector<Projectile*> m_player_projectiles;
	EnemySpawner* m_enemy_spawner;

	SpaceBackground* m_bg;

};

