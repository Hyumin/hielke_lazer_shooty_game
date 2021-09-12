#include "Game.h"
#include <math.h>
#include "..\Engine\ManagerSingleton.h"
#include "..\Game\AI\EnemySpawner.h"

using namespace Hielke;


Game::Game()
{
	m_ResMan = NULL;
	Init();
}
Game::Game(ResourceManager* _resMan)
{
	m_ResMan = _resMan;
	Init();
}

Game::~Game()
{
	delete m_enemy_spawner;
	m_enemy_spawner = NULL;
	m_ResMan = NULL;
	delete m_player_cannon;
	m_player_cannon = NULL;

	m_enemies.clear();
	m_player_projectiles.clear();
}

void Game::Init()
{
	m_player_cannon = new Cannon(300, 400);
	m_kup, m_kdwn, m_ksht = false;
	m_enemy_spawner = new EnemySpawner();
	m_enemy_spawner->Initialize(this, m_ResMan);

	//SpawnBalls(1);

}



void Game::SetResourceManager(ResourceManager* _resMan)
{
	m_ResMan = _resMan;
}

void Game::Update(float _dt)
{
	m_player_cannon->Update(_dt);
	m_enemy_spawner->Update(_dt);
	if (m_kup)
	{
		m_player_cannon->Rotate(-5*_dt);
	}
	if (m_kdwn)
	{
		m_player_cannon->Rotate(5*_dt);
	}
	if (m_ksht)
	{
		Projectile* proj = m_player_cannon->Shoot();
		if (proj != nullptr)
		{
			m_player_projectiles.push_back(proj);
		}
	}
	for (unsigned i = 0; i < m_enemies.size(); ++i)
	{
		m_enemies[i]->Update(_dt);
		//Check collision with the enemies
		for (uint32_t j = 0; j < m_player_projectiles.size(); ++j)
		{
			/*Check here if untargetable as its less cost efficient compared to the collision check*/
			if (!m_player_projectiles[j]->m_untargatable)
			{
				if (Box::BoxCollision(m_player_projectiles[j]->m_box, m_enemies[i]->m_collider))
				{
					m_enemies[i]->TakeDamage(0.01f);
					m_player_projectiles[j]->Die();
				}
			}
		}
		//If enemy is supposed to die delete em.
		if (m_enemies[i]->GetDeathState())
		{
			m_enemies.erase(m_enemies.begin() + i);
			//SpawnBalls(2);
		}
	}
	Vector2 player_pos = m_player_cannon->Get_Position();
	for (uint32_t i = 0; i < m_player_projectiles.size(); ++i)
	{
		m_player_projectiles[i]->Update(_dt);
		Vector2 bul_pos = m_player_projectiles[i]->GetPos();
		//if(Box::BoxCollision(m_player_projectiles[i]->m_box,m_enemies[i].get))

		if (m_player_projectiles[i]->GetDelete())
		{
			m_player_projectiles.erase(m_player_projectiles.begin() + i);
		}
		//Hard erase
		else if (Vector2::Distance(player_pos, bul_pos) > 3000)
		{
			m_player_projectiles.erase(m_player_projectiles.begin() + i);
		}
		
	}

}

void Game::KeyDown(unsigned int _key)
{
	if (_key == SDLK_w)
	{
		m_kup = true;
	}
	if (_key == SDLK_s)
	{
		m_kdwn = true;
	}
	if (_key == SDLK_SPACE)
	{
		m_ksht = true;
	}
}

void Game::SpawnBalls(int _num_balls)
{
	EnemyStats mad_eye_stats;
	mad_eye_stats.acceleration = 0.00f;
	mad_eye_stats.current_health = 200;
	mad_eye_stats.max_health = 200;

	for (int i = 0; i < _num_balls; ++i)
	{
		MadEye* henk = new MadEye(random_range(0,1200), random_range(0, 800), mad_eye_stats);
		henk->SetTarget(m_player_cannon);
		m_enemies.push_back(henk);
	}
}

void Game::ToggleDebugMode()
{
	m_DebugMode = m_DebugMode ? false : true;

	m_player_cannon->DebugMode(m_DebugMode);
	for (unsigned int i = 0; i < m_enemies.size(); ++i)
	{
		m_enemies[i]->m_debug = m_DebugMode;
	}

}

void Game::KeyUp(unsigned int _key)
{
	//toggles debugmode
	if (_key == m_debug_mode_key)
	{
		ToggleDebugMode();
	}

	if (_key == SDLK_w)
	{
		m_kup = false;
	}
	if (_key == SDLK_s)
	{
		m_kdwn = false;
	}
	if (_key == SDLK_SPACE)
	{
		m_ksht = false;
	}
}

void Game::Render(SDLRenderer* _renderer)
{
	//m_DebugInfo->Render(_renderer);
	m_player_cannon->draw(_renderer);
	
	m_WindowSize.x = (float)_renderer->GetWindowWidth();
	m_WindowSize.y = (float)_renderer->GetWindowHeight();

	for (unsigned i = 0; i < m_enemies.size(); ++i)
	{
		m_enemies[i]->Render(_renderer);
	}
	for (unsigned i = 0; i < m_player_projectiles.size(); ++i)
	{
		m_player_projectiles[i]->Render(_renderer);
	}
}

void Game::AddEnemey(Enemy* _enem)
{
	_enem->m_debug = m_DebugMode;
	m_enemies.push_back(_enem);
}
