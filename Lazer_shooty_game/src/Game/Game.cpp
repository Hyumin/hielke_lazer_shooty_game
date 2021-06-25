#include "Game.h"
#include <math.h>

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
	m_player_cannon = new Cannon(300, 300);

}



void Game::SetResourceManager(ResourceManager* _resMan)
{
	m_ResMan = _resMan;
}

void Game::Update(float _dt)
{
	m_player_cannon->Update(_dt);
}

void Game::KeyDown(unsigned int _key)
{
	if (_key == SDLK_w)
	{
		m_player_cannon->Rotate(0.1f);
	}
	if (_key == SDLK_s)
	{
		m_player_cannon->Rotate(-0.1f);
	}
	if (_key == SDLK_SPACE)
	{
		m_player_cannon->Shoot();
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
}

void Game::Render(SDLRenderer* _renderer)
{
	//m_DebugInfo->Render(_renderer);
	m_player_cannon->draw(_renderer);
	
	m_WindowSize.x = (float)_renderer->GetWindowWidth();
	m_WindowSize.y = (float)_renderer->GetWindowHeight();
}
