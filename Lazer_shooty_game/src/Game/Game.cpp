#include "Game.h"
#include <math.h>
#include "..\Engine\ManagerSingleton.h"

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
	m_ResMan = NULL;
	delete m_player_cannon;
	m_player_cannon = NULL;
}

void Game::Init()
{
	m_player_cannon = new Cannon(300, 400);
	m_test_enemy = Object({ 800 , 100 }, {64,64});
	m_test_enemy.m_RenderInterface.texture = ManagerSingleton::getInstance().res_man->LoadTexture("Assets//SpriteSheets//enemy//mad_eyeball_man.png");
	m_kup, m_kdwn, m_ksht = false;

}



void Game::SetResourceManager(ResourceManager* _resMan)
{
	m_ResMan = _resMan;
}

void Game::Update(float _dt)
{
	m_player_cannon->Update(_dt);
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
		m_player_cannon->Shoot();
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

void Game::ToggleDebugMode()
{
	m_DebugMode = m_DebugMode ? false : true;

	m_player_cannon->DebugMode(m_DebugMode);
	if (m_DebugMode)
	{
	}
	else
	{
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

	m_test_enemy.Render(_renderer, { 0,0 });
}
