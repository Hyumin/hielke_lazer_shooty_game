#include "SDLRenderer.h"

#include "ResourceManager.h"
#include "RenderTarget.h"
#include "Layer.h"
#include <iostream>

using namespace std;

SDLRenderer::SDLRenderer()
{
	if (!Init(m_WindowName,m_WindowWidth,m_WindowHeight))
	{
		printf("Initialization has failed \n");
	}
}

SDLRenderer::SDLRenderer(std::string _name, unsigned int _width, unsigned int _height)
{
	if (!Init(_name, _width, _height))
	{
		printf("Initialization has failed \n");
	}
}

SDLRenderer::~SDLRenderer()
{
	CleanUp();
}

void SDLRenderer::AddToRenderTarget(RenderInterface _interface, unsigned int _layer, unsigned int _target)
{

}

void SDLRenderer::AddToRenderqueue(RenderInterface _interface, unsigned int _layer )
{
	m_Layers[_layer].AddInterface(_interface);
}

void SDLRenderer::AddToRenderqueue(TextRenderInterface _interface, unsigned int _layer )
{
	m_Layers[_layer].AddText(_interface);
}

void SDLRenderer::AddLine(const Vector2& _a, const Vector2& _b, const Vector2& _worldPos, SDL_Color _color, unsigned int _layer )
{
	Line newLine;
	newLine.start = _a;
	newLine.end = _b;
	newLine.start -= _worldPos;
	newLine.end -= _worldPos;


	newLine.colour = _color;

	m_Layers[_layer].AddLine(newLine);
}

void SDLRenderer::AddLine(const Line& _line, const Vector2& _worldPos, unsigned int _layer)
{
	Line newLine= _line;
	newLine.start -= _worldPos;
	newLine.end -= _worldPos;


	m_Layers[_layer].AddLine(newLine);
}

void SDLRenderer::AddLineZoomed(const Vector2& _a, const Vector2& _b, const Vector2& _worldPos, SDL_Color _color, float _zoom, unsigned int _layer)
{
	Line newLine;
	newLine.start = _a*_zoom;
	newLine.end = _b*_zoom;
	newLine.start -= _worldPos*_zoom;
	newLine.end -= _worldPos*_zoom;


	newLine.colour = _color;

	m_Layers[_layer].AddLine(newLine);
}

void SDLRenderer::AddLineZoomed(const Line& _line, const Vector2& _worldPos, float _zoom, unsigned int _layer)
{
	Line newLine = _line;
	newLine.start *=  _zoom;
	newLine.end *=  _zoom;
	newLine.start -= _worldPos * _zoom;
	newLine.end -= _worldPos * _zoom;



	m_Layers[_layer].AddLine(newLine);
}

void SDLRenderer::Render()
{
	if (m_ResMan != nullptr)
	{

		SDL_Rect txtsrc= { 0,0,m_FPSWidth,m_FPSHeight };
		SDL_Rect txtdest = { 0,0,m_FPSWidth,m_FPSHeight };
		
		//Clear screen
		SDL_SetRenderDrawColor(m_Renderer, 0x0, 0x8D, 0xAD, 0xff);
		SDL_RenderClear(m_Renderer);
		for (unsigned int l = 0; l < m_Layers.size(); ++l)
		{
			std::vector<FilledBox>& boxes = m_Layers[l].GetFilledBoxQueue();

			for (int i = 0; i < boxes.size(); ++i)
			{
				SDL_SetRenderDrawColor(m_Renderer, boxes[i].col.r, boxes[i].col.g, boxes[i].col.b, boxes[i].col.a);
				SDL_RenderFillRect(m_Renderer, &boxes[i].box);
			}
			std::vector<Line>& lines = m_Layers[l].GetLineQueue();

			for (int i = 0; i < lines.size(); ++i)
			{
				Line l = lines[i];

				SDL_SetRenderDrawColor(m_Renderer, l.colour.r, l.colour.g, l.colour.b, l.colour.a);
				SDL_RenderDrawLine(m_Renderer, (int)l.start.x, (int)l.start.y, (int)l.end.x, (int)l.end.y);
			}
			std::vector<WireFrameBox>& wfBoxes = m_Layers[l].GetWireFrameBoxes();

			for (int i = 0; i < wfBoxes.size(); ++i)
			{
				SDL_SetRenderDrawColor(m_Renderer, wfBoxes[i].col.r, wfBoxes[i].col.g, wfBoxes[i].col.b, wfBoxes[i].col.a);
				SDL_Rect rect;
				rect.x = wfBoxes[i].box.pos.x;
				rect.y = wfBoxes[i].box.pos.y;
				rect.w = wfBoxes[i].box.w;
				rect.h = wfBoxes[i].box.h;
				SDL_RenderDrawRect(m_Renderer, &rect);
			}

			
			std::vector<TextRenderInterface>& textInterfaces = m_Layers[l].GetTextRenderQueue();

			//Iterate through text renderer
			for (int i = 0; i < textInterfaces.size(); ++i)
			{
				TextRenderInterface inter = textInterfaces[i];
				const SDL_RendererFlip flip = inter.renderFlip;
				if (inter.texture != nullptr)
				{
					SDL_RenderCopyEx(m_Renderer, inter.texture, &inter.srcRect, &inter.destRect, 0, 0, flip);
				}
			}
			std::vector<RenderInterface>& renderInterfaces = m_Layers[l].GetRenderQueue();
			//Iterate through the render queue
			for (int i = 0; i < renderInterfaces.size(); ++i)
			{
				RenderInterface inter = renderInterfaces[i];
				Texture* tex = inter.texture;
				if (tex != nullptr)
				{
					//SDL_point is only used for SDL_rotate
					SDL_RenderCopyEx(m_Renderer, tex->GetTexture(), &inter.srcRect, &inter.destRect, 0, 0, inter.renderFlip);
				}
			}

			std::vector<RenderTarget*>& renderTargets = m_Layers[l].GetRenderTargets();
			for (unsigned int i = 0; i < renderTargets.size(); ++i)
			{
				SDL_Texture* tex =  renderTargets[i]->GetTexture();
				if (tex != nullptr)
				{
					//Change render target to be the texture of this class
					SDL_SetRenderTarget(m_Renderer, renderTargets[i]->GetTexture());
					renderTargets[i]->Render(m_ResMan);
					SDL_SetRenderTarget(m_Renderer, NULL);
					//The src rectangle is the dest rect without the position
					SDL_Rect srcreect = renderTargets[i]->GetDestRect();
					srcreect.x = 0;
					srcreect.y = 0;
					SDL_Rect destrect = renderTargets[i]->GetDestRect();
					SDL_RenderCopyEx(m_Renderer, renderTargets[i]->GetTexture(), &srcreect, &destrect,0,nullptr,SDL_RendererFlip::SDL_FLIP_NONE);
				}
			}
			//Put target back to default render target
			SDL_SetRenderTarget(m_Renderer, NULL);


			m_Layers[l].ClearQueues();

		
		}
		//Present screen
		SDL_RenderPresent(m_Renderer);
	}
	else
	{
		printf("SDLRenderer: Resourcemanager was not set but the render function requires one to work \n");
	}

}

void SDLRenderer::Update(float _dt)
{


}

void SDLRenderer::SetWindowWidth(unsigned int _width)
{
	m_WindowWidth = _width;
	//Resize window
}

void SDLRenderer::SetWindowHeight(unsigned int _height)
{
	m_WindowHeight = _height;
	//Resize window
	
}

void SDLRenderer::SetResourceManager(ResourceManager* _resman)
{
	m_ResMan = _resman;
}

void SDLRenderer::DrawBox(Box _box, SDL_Color _color, Vector2 _worldPos, unsigned int _layer )
{
	WireFrameBox b;
	b.box = _box;
	b.col = _color;
	b.thickness = 1;
	b.box.pos  -= _worldPos;
	m_Layers[_layer].AddBox(b);

}

void SDLRenderer::DrawBox(int _x, int _y, int _w, int _h, SDL_Color _color, Vector2 _worldPos, unsigned int _layer)
{
	WireFrameBox b;
	b.box = Box{(float)_x,(float)_y,(float)_w,(float)_h};
	b.col = _color;
	b.thickness = 1;
	b.box.pos -= _worldPos;
	m_Layers[_layer].AddBox(b);
}

void SDLRenderer::DrawFilledBox(int _x, int _y, int _w, int _h, SDL_Color _color, Vector2 _worldPos, unsigned int _layer )
{
	FilledBox b;
	b.box = { _x-(int)_worldPos.x,_y-(int)_worldPos.y,_w,_h };
	b.col = _color;
	m_Layers[_layer].AddFilledBox(b);
}

void SDLRenderer::DrawFilledBox(Box _box, SDL_Color _color, Vector2 _worldPos, unsigned int _layer)
{
	FilledBox b;
	b.box = {(int) _box.pos.x - (int)_worldPos.x,(int)_box.pos.y - (int)_worldPos.y,(int)_box.w,(int)_box.h };
	b.col = _color;
	m_Layers[_layer].AddFilledBox(b);
}

void SDLRenderer::DrawBoxZoomed(Box _box, SDL_Color _color, Vector2 _worldPos, float _zoom, unsigned int _layer)
{
	WireFrameBox b;
	b.box = _box;
	b.col = _color;
	b.thickness = 1;
	b.box.pos *= _zoom;
	b.box.pos -= _worldPos* _zoom;
	b.box.w *= _zoom;
	b.box.h *= _zoom;
	m_Layers[_layer].AddBox(b);
}

void SDLRenderer::DrawBoxZoomed(int _x, int _y, int _w, int _h, SDL_Color _color, Vector2 _worldPos, float _zoom, unsigned int _layer)
{
	WireFrameBox b;
	b.box = Box{ (float)_x,(float)_y,(float)_w,(float)_h };
	b.col = _color;
	b.thickness = 1;
	b.box.pos *= _zoom;
	b.box.pos -= _worldPos * _zoom;
	b.box.w *= _zoom;
	b.box.h *= _zoom;
	m_Layers[_layer].AddBox(b);
}

void SDLRenderer::DrawFilledBoxZoomed(int _x, int _y, int _w, int _h, SDL_Color _color, Vector2 _worldPos, float _zoom, unsigned int _layer)
{
	
	FilledBox b;
	b.box = { _x - (int)(_worldPos.x * _zoom),_y - (int)(_worldPos.y * _zoom),(int)(_w*_zoom),(int)(_h*_zoom) };
	b.box.x = (int)(_zoom *b.box.x);
	b.box.y = (int)(_zoom* b.box.y);
	b.col = _color;
	m_Layers[_layer].AddFilledBox(b);
}

void SDLRenderer::DrawFilledBoxZoomed(Box _box, SDL_Color _color, Vector2 _worldPos, float _zoom, unsigned int _layer)
{
	FilledBox b;
	b.box = { (int)_box.pos.x - (int)(_worldPos.x * _zoom),(int)_box.pos.y - (int)(_worldPos.y * _zoom),(int)(_box.w*_zoom),(int)(_box.h*_zoom) };
	b.box.x = (int)(_zoom * b.box.x);
	b.box.y = (int)(_zoom * b.box.y);
	b.col = _color;
	m_Layers[_layer].AddFilledBox(b);
}

RenderTarget* SDLRenderer::CreateRenderTarget(Box _box,unsigned int _layer)
{
	RenderTarget* t = new RenderTarget(m_Renderer);
	t->CreateTexture(_box);

	m_Layers[_layer].GetRenderTargets().push_back(t);
	return t;
}

bool SDLRenderer::Init(std::string _name, unsigned int _width, unsigned int _height)
{
	bool succes = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not be initialized SDL_ERROR: %s \n", SDL_GetError());
		succes = false;
	}
	else //SDL initialization succesfull
	{
		//Create a window
		m_Window = SDL_CreateWindow(_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_SHOWN);
		if (m_Window == NULL)
		{
			printf("Window couold not be created! SDL_ERROR: %s \n", SDL_GetError());
			succes = false;
		}
		else
		{
			//Assign height,width and name to their variables
			m_WindowName = _name;
			m_WindowWidth = _width;
			m_WindowHeight = _height;

			//Create renderer for window allowing textures to be used so we can utilise the gpu instead of the cpu for graphics
			m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
			if (m_Renderer == NULL)
			{
				printf("Window renderer  could not be created! SDL_ERROR: %s\n", SDL_GetError());
				succes = false;
			}
			else
			{
				SDL_SetRenderDrawColor(m_Renderer, 0x0, 0x8D, 0xAD, 0xff);

				//Initialize SDL_Image
				int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

				//IMG_init returns  the flags it initialized or 0 on failure, and operation with the flags will allow us to check whether 
				//the correct fileformat has been loaded
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					succes = false;
				}
				else
				{
					//succes

					
				}
				if (TTF_Init() == -1)
				{
					printf("Failed to initailize! SDL_ttf Error: %s \n", TTF_GetError());
				}
				else
				{
					//Load in default font
					m_DefaultFont = TTF_OpenFont("Assets//Fonts//Jupiter.ttf", 28);
					if (m_DefaultFont == NULL)
					{
						printf("Could not load defaultfont \n");
					}
				}
			}

			m_Layers.push_back(Layer("Default"));
			m_Layers.push_back(Layer("Background"));
			m_Layers.push_back(Layer("UI"));
			m_Layers.push_back(Layer("Editor_1"));
			m_Layers.push_back(Layer("Editor_2"));
			m_Layers.push_back(Layer("Editor_3"));
			m_Layers.push_back(Layer("DEBUG"));
		}
	}

	return succes;
}

void SDLRenderer::CleanUp()
{
	m_ResMan = NULL;
	SDL_DestroyRenderer(m_Renderer);
	m_Renderer = NULL;
	SDL_DestroyWindow(m_Window);
	m_Window = NULL;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
