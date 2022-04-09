
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <time.h>

#include "Engine\ResourceManager.h"
#include "Game\Game.h"
#include "Engine\SDLRenderer.h"
#include "Editor\Editor.h"
#include "Engine\ManagerSingleton.h"


//Made using https://lazyfoo.net/tutorials/SDL/index.php

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const char* WINDOWNAME = "Hielke's cool windowname ";


bool init();

void close();


SDLRenderer* g_Renderer= NULL;
ResourceManager* g_Resmanager = NULL;
Game* g_Game = NULL;
Editor* g_Editor = NULL;


bool EDITORMODE = false;


int main(int arg, char* args[])
{
	bool quit = false;
	SDL_Event e;
	// 
	float start = 0.0;
	float end = 0.0f;
	float timer = 0.0f;
	srand(time(NULL));

	if (!init())
	{
		printf(" Failed to initialize SDL \n");
		close();
		return 1;
	}
	else
	{
		start =(float) SDL_GetTicks();
		//Main loop
		while (!quit)
		{
			start = end;
			end = (float)SDL_GetTicks();
			float dt = (end - start) / 1000.0f;
			//Poll events
			while (SDL_PollEvent(&e) !=0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					if (g_Editor != nullptr)
					{
						g_Editor->KeyDown(e.key.keysym.sym);
					}
					if (g_Game != nullptr)
					{
						g_Game->KeyDown(e.key.keysym.sym);
					}
					switch (e.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						quit = true;
						break;
					}
				}
				else if (e.type == SDL_KEYUP)
				{
					if (g_Editor != nullptr)
					{
						g_Editor->KeyUp(e.key.keysym.sym);
					}
					if(g_Game!=nullptr)
					{
						g_Game->KeyUp(e.key.keysym.sym);
					}
				}
				else if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					if (g_Editor != nullptr)
					{
						g_Editor->MouseDown(e.button.button);
					}
					if (g_Game != nullptr)
					{
						g_Game->MouseDown(e.button.button);
					}
				}
				else if (e.type == SDL_MOUSEBUTTONUP)
				{
					if (g_Editor != nullptr)
					{
						g_Editor->MouseUp(e.button.button);
					}
					if (g_Game != nullptr)
					{
						g_Game->MouseUp(e.button.button);
					}
				}
				else if (e.type == SDL_MOUSEMOTION)
				{
					//Get mouse position
					int x, y;
					SDL_GetMouseState(&x, &y);
					if (g_Editor !=nullptr)
					{
						g_Editor->MouseMove(x, y);
					}
					if (g_Game != nullptr)
					{
						g_Game->MouseMove(x, y);
					}
				}
				else if (e.type == SDL_MOUSEWHEEL)//https://wiki.libsdl.org/SDL_MouseWheelEvent
				{
					//pass on mousewheel events
					if (g_Editor != nullptr)
					{
						g_Editor->MouseWheel(e.wheel.x, e.wheel.y);
					}
				}
			}
			// if sprite editor mode launch sprite editor
			if (g_Editor!=nullptr)
			{
				g_Editor->Update(dt);
				g_Editor->Render(g_Renderer);
			}
			if(g_Game!=nullptr)
			{
				g_Game->Update(dt);
				g_Game->Render(g_Renderer);
			}
			g_Renderer->Update(dt);
			g_Renderer->Render();	
		}
		
	}
	close();

	return 0;
}



bool init()
{
	bool succes = true;
	
	g_Renderer = new SDLRenderer(std::string(WINDOWNAME), SCREEN_WIDTH, SCREEN_HEIGHT);
	g_Resmanager = new ResourceManager(g_Renderer);
	g_Renderer->SetResourceManager(g_Resmanager);
	g_Editor = nullptr;
	g_Game = nullptr;
	ManagerSingleton::getInstance().res_man = g_Resmanager;

	if (EDITORMODE)
	{
		g_Editor = new Editor(g_Resmanager);
		g_Editor->OnWindowResize(SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	else
	{
		g_Game = new Game(g_Resmanager);
	}

	return succes;
}


//unloads and closes sdl systeems
void close()
{
	delete g_Game;
	g_Game = NULL;
	
	delete g_Editor;
	g_Editor = NULL;

	delete g_Resmanager;
	g_Resmanager = NULL;

	delete g_Renderer;
	g_Renderer = NULL;
	
	ManagerSingleton::getInstance().res_man = nullptr;

}

