#include "TextField.h"
#include "RenderTarget.h"
TextField::TextField()
{
	m_Texture = NULL;
	m_Colour.a = 0xff;
	m_Colour.b = 0xaa;
	m_Colour.g = 0xaa;
	m_Colour.r = 0xaa;
	m_Text = "Not set text lol! ";
	m_FontPointer = NULL;
}

TextField::~TextField()
{
}

void TextField::Update(float _dt)
{
}

void TextField::Render(SDLRenderer* _renderer, Vector2 _worldpos,int _layer)
{
	//If we're dealing with text that has nothing in it  just return
	if (m_Text.length() == 0)
	{
		return;
	}

	if (m_FontPointer != NULL)
	{
		UpdateInterface(_renderer);
		m_Interface.destRect.x -= (int)_worldpos.x;
		m_Interface.destRect.y -= (int)_worldpos.y;
		_renderer->AddToRenderqueue(m_Interface,_layer);
	}
	else
	{
		throw std::exception("No font assigned to text can't reasonably do shit now :( \n");
	}
}

void TextField::RenderToTarget(SDLRenderer* _renderer,RenderTarget* _target, Vector2 _worldpos)
{
	//If we're dealing with text that has nothing in it  just return
	if (m_Text.length() == 0)
	{
		return;
	}

	if (m_FontPointer != NULL)
	{
		UpdateInterface(_renderer);
		m_Interface.destRect.x -= (int)_worldpos.x;
		m_Interface.destRect.y -= (int)_worldpos.y;
		_target->AddText(m_Interface);
	}
	else
	{
		throw std::exception("No font assigned to text can't reasonably do shit now :( \n");
	}
}



void TextField::SetText(const std::string& _string)
{
	m_Text = _string;
}

void TextField::SetText(const char* _text)
{
	m_Text = std::string(_text);
}

void TextField::SetFont(TTF_Font* _font)
{
	m_FontPointer = _font;
}

void TextField::SetColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a)
{
	m_Colour.r = _r;
	m_Colour.g = _g;
	m_Colour.b = _b;
	m_Colour.a = _a;
}

void TextField::SetColour(const SDL_Color& _color)
{
	m_Colour = _color;
}
