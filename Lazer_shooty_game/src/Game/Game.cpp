#pragma once
#include "Game.h"
#include <math.h>
#include "..\Engine\ManagerSingleton.h"
#include "..\Game\AI\EnemySpawner.h"
#include "AI/Heary.h"
#include "AI/MadEye.h"

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
	delete m_bg;
	m_bg = NULL;

	delete m_LazerExplosion;
	m_LazerExplosion= NULL;

}

void Game::Init()
{
	m_player_cannon = new Cannon(100, 980);
	m_kup, m_kdwn, m_ksht = false;
	m_enemy_spawner = new EnemySpawner();
	m_enemy_spawner->Initialize(this, m_ResMan);
	m_bg = new SpaceBackground({ 10,12,45,255 }, { 1920 ,1080 });
	m_LazerExplosion = new SoundEffect();
	m_LazerExplosion->LoadEffect("Assets//Audio//sound_effects//simple_shoot_explosion.wav");
	//SpawnBalls(10);

}



void Game::SetResourceManager(ResourceManager* _resMan)
{
	m_ResMan = _resMan;
}

void Game::Update(float _dt)
{
	m_player_cannon->Update(_dt);
	m_enemy_spawner->Update(_dt);
	m_bg->Update(_dt);
	UpdateEnemies(_dt);
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
			proj->m_debug = m_DebugMode;
			m_player_projectiles.push_back(proj);
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
	mad_eye_stats.current_health = 200000000;
	mad_eye_stats.max_health = 200000000;

	for (int i = 0; i < _num_balls; ++i)
	{
		MadEye* henk = new MadEye(random_range(0,1920), random_range(0, 800), mad_eye_stats);
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

	for (uint32_t i = 0; i < m_player_projectiles.size(); ++i)
	{
		m_player_projectiles[i]->ToggleDebug();
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
	if (_key == SDLK_2)
	{
		m_player_cannon->ToggleMouseMode();
	}
	if (_key == SDLK_9)
	{
		for (int i =0; i <10; ++i)
		m_player_cannon->m_stats->LevelUp();
	}
}

void Game::MouseMove(int _x, int _y)
{

	m_MousePos.x = (float)_x;
	m_MousePos.y = (float)_y;
	m_player_cannon->MouseMove(m_MousePos);
}

// 1 is left mouse, 2 is middle 3 is right
void Game::MouseUp(int _key)
{
	if (_key == 1)
	{
		m_ksht = false;
	}
}

void Game::MouseDown(int _key)
{
	if (_key == 1)
	{
		m_ksht = true;
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
	m_bg->Render(_renderer);
}

void Game::AddEnemey(Enemy* _enem)
{
	_enem->m_debug = m_DebugMode;
	m_enemies.push_back(_enem);
}

/**
* Updates enemies and checks collision with the player projectiles.
*/
void Game::UpdateEnemies(float _dt)
{
	for (unsigned i = 0; i < m_enemies.size(); ++i)
	{
		m_enemies[i]->Update(_dt);
		for (uint32_t j = 0; j < m_player_projectiles.size(); ++j)
		{
			/*Check here if untargetable as its less cost efficient compared to the collision check*/
			if (!m_player_projectiles[j]->m_untargatable)
			{
				if (CircleToCircleCollision(m_player_projectiles[j]->m_Circle, m_enemies[i]->m_circle))
				{
					//Take dmg returns a boolean whether or not the enemy is dead after that 
					//If its true the enemy died if its false its still alive
					if (m_enemies[i]->TakeDamage(m_player_projectiles[j]->GetDamage()))
					{
						m_player_cannon->m_stats->AddExp(random_range(1, 1000));
					}
					m_player_projectiles[j]->Die();
					m_LazerExplosion->PlayEffect();
				}
			}
		}
		//If enemy is supposed to die delete em.
		if (m_enemies[i]->GetDeathState())
		{
			m_enemies.erase(m_enemies.begin() + i);
		}
	}
}
