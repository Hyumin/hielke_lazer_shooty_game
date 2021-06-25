#include "Button.h"
#include <iostream>
#include "RenderTarget.h"

Button::Button()
{
	Init();

}

Button::~Button()
{
}

void Button::Update(float _dt)
{

}

void Button::MouseDown(unsigned int _key)
{
	if (_key == 1)
	{
		if (m_Hovered) // once its hoeverd we know we're already colliding with 
		{
			m_Clicked = true;
		}
	}
}

void Button::MouseMove(int _x, int _y)
{
	m_MousePos.x = (float)_x;
	m_MousePos.y = (float)_y;

	if (m_Collider.BoxCollision(m_Collider, m_MousePos))
	{
		m_Hovered = true;
	}
	else
	{
		m_Hovered = false;
	}

}

void Button::MouseUp(unsigned int _key)
{
	if (_key == 1)
	{
		if (m_Clicked) 
		{
			m_Clicked = false;
			if (m_IntCallback != nullptr)
			{
				m_IntCallback(m_IntCallbackInput);
			}
			//Fire event
			if (m_Callback != nullptr)
			{
				m_Callback();
			}
			
		}	
	}
}

void Button::Render(SDLRenderer* _renderer)
{
	
	SDL_Colour col;
	//If we don't use texture drawing mode use colours to define how 
	// the button will be rendered
	if (m_DrawMode != TEXTURE)
	{
		col = m_ColNorm;
		if (m_Hovered)
		{
			col = m_ColHov;
		}
		if (m_Clicked)
		{
			col = m_ColClicked;
		}
	}
	//Draw based on whatever draw mode
	switch (m_DrawMode)
	{
	case WIREFRAME:
		_renderer->DrawBox(m_Collider, col, { 0,0 }, m_Layer);
		if (m_TextField.GetText().size() > 0)
		{
			m_TextField.Render(_renderer, { 0,0 }, m_Layer + 1);
		}
		break;
	case FILLEDRECT:
		_renderer->DrawFilledBox(m_Collider, col, { 0,0 }, m_Layer);
		if (m_TextField.GetText().size() > 0)
		{
			m_TextField.Render(_renderer, { 0,0 }, m_Layer + 1);
		}
		break;
	case TEXTURE:
		//Assign render interface to the object based on the current state of the button
		m_Object.m_RenderInterface = m_TextureNorm;
		if (m_Hovered)
		{
			m_Object.m_RenderInterface = m_TexHovered;
		}
		if (m_Clicked)
		{
			m_Object.m_RenderInterface = m_TexClicked;
		}
		m_Object.Render(_renderer, { 0,0 }, m_Layer);
		break;
	default:
		printf("Unspecified button draw mode in button.cpp \n");
		break;
	}
}

void Button::RenderToTarget(SDLRenderer* _renderer, RenderTarget* _target,Vector2 _offset)
{
	SDL_Colour col;
	//If we don't use texture drawing mode use colours to define how 
	// the button will be rendered
	if (m_DrawMode != TEXTURE)
	{
		col = m_ColNorm;
		if (m_Hovered)
		{
			col = m_ColHov;
		}
		if (m_Clicked)
		{
			col = m_ColClicked;
		}
	}
	//Draw based on whatever draw mode
	switch (m_DrawMode)
	{
	case WIREFRAME:
		{
			WireFrameBox wb;
			wb.thickness = 1;
			wb.box = m_Collider;
			wb.col = col;
			_target->AddBox(wb);
			if (m_TextField.GetText().size() > 0)
			{
				m_TextField.RenderToTarget(_renderer, _target, _offset);
			}
		}
		break;
	case FILLEDRECT:
		FilledBox b;
		b.box = {(int)m_Collider.pos.x,(int)m_Collider.pos.y,(int)m_Collider.w,(int)m_Collider.h};
		b.col = col;
		_target->AddFilledBox(b);
		if (m_TextField.GetText().size() > 0)
		{
			m_TextField.RenderToTarget(_renderer,_target, _offset);
		}
		break;
	case TEXTURE:
		{
			//Assign render interface to the object based on the current state of the button
			m_Object.m_RenderInterface = m_TextureNorm;
			if (m_Hovered)
			{
				m_Object.m_RenderInterface = m_TexHovered;
			}
			if (m_Clicked)
			{
				m_Object.m_RenderInterface = m_TexClicked;
			}
			RenderInterface itf = m_Object.m_RenderInterface;
			itf.destRect.x -= _offset.x;
			itf.destRect.y -= _offset.y;
			_target->AddInterface(itf);
		}
		break;
	default:
		printf("Unspecified button draw mode in button.cpp \n");
		break;
	}
}

void Button::SetTextureDrawMode(RenderInterface _norm, RenderInterface _clicked, RenderInterface _hovered)
{
	m_TextureNorm = _norm;
	m_TexClicked = _clicked;
	m_TexHovered = _hovered;
	m_DrawMode = TEXTURE;
}

void Button::SetTextureDrawModeWithSheet(Texture* _tex, SDL_Rect _norm, SDL_Rect _clicked, SDL_Rect _hovered, SDL_RendererFlip _renderFlip)
{
	RenderInterface n, c, h;
	n = { _norm,{0,0,0,0},_tex, _renderFlip,{0,0} };
	c = n;
	h = n;

	c.srcRect = _clicked;
	h.srcRect = _hovered;
	m_TextureNorm = n;
	m_TexClicked = c;
	m_TexHovered = h;
	m_DrawMode = TEXTURE;
}

void Button::SetFilledRectMode(SDL_Colour _normal, SDL_Colour _hovered, SDL_Colour _clicked)
{
	m_ColNorm = _normal;
	m_ColHov = _hovered;
	m_ColClicked = _clicked;
	m_DrawMode = FILLEDRECT;
	m_TextField.SetColour(_clicked);
}

//In case you want to re-use the same colours as wireframe mode
void Button::SetFilledRectMode()
{
	m_DrawMode = FILLEDRECT;
}

void Button::SetWireFrameMode(SDL_Colour _normal, SDL_Colour _hovered, SDL_Colour _clicked)
{
	m_ColNorm = _normal;
	m_ColHov = _hovered;
	m_ColClicked = _clicked;
	m_DrawMode = WIREFRAME;
	m_TextField.SetColour(_clicked);
}

//In case you just want to re-use the same colours as filled rect mode
void Button::SetWireFrameMode()
{
	m_DrawMode = WIREFRAME;
}

void Button::SetText(const std::string& _text)
{
	m_TextField.SetText(_text);
}

void Button::SetLayer(int _layer)
{
	m_Layer = _layer;
}

void Button::SetSize(Vector2 _size)
{
	m_Size = _size;
	m_Object.m_Size = _size;
	m_Collider.w = _size.x;
	m_Collider.h = _size.y;
	m_TextField.m_Size = _size;
}

void Button::SetPosition(Vector2 _pos)
{
	m_Pos = _pos;
	m_Object.m_Pos = m_Pos;
	m_Collider.pos = m_Pos;
	m_TextField.m_pos = m_Pos;

}

void Button::SetCallbackFunction(std::function<void()> fnc)
{
	m_Callback = fnc;
}

void Button::SetCallbackFunction(std::function<void(int)> fnc, int _int)
{
	m_IntCallback = fnc;
	m_IntCallbackInput = _int;
}

void Button::SetFont(TTF_Font* _Font)
{
	m_Font = _Font;
	m_TextField.SetFont(m_Font);
}

void Button::SetTextColor(SDL_Color _col)
{
	m_TextField.SetColour(_col);
}

void Button::Init()
{
	m_Hovered = false;
	m_Clicked = false;
	m_Layer = 1;
	m_MousePos = { 0,0 };
	m_TexHovered = {};
	m_TextureNorm = { };
	m_TexClicked = {};
	m_Font = nullptr;
	m_Object = Object{};
	m_Pos = Vector2{ 0,0 };
	m_Collider = Box(0, 0, 100, 100);
	m_Callback = NULL;
	m_IntCallback = NULL;
	m_IntCallbackInput = -1;
	m_TextField = TextField{};
	m_TextField.SetText("");//Set text to nothing so it won't render it 
	SetFilledRectMode({ 125,125,125,255 }, { 255,255,255,255 }, { 0,0,0,255 });


}
