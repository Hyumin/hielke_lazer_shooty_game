#include "NumberedBox.h"

NumberedBox::NumberedBox(int _number)
{
	Init();
	SetNumber(_number);
}

NumberedBox::~NumberedBox()
{

}


void NumberedBox::Init()
{
	m_Box = Box();
	m_Colour = { 255,255,255,255 };
	m_Text = TextField();
}

void NumberedBox::UpdateTextField()
{
	//Puts the textfield in the appropiate corner
	//For starters its gonna be in the top left corner
	m_Text.m_pos = m_Box.pos;
	m_Text.m_Size = { m_Box.w ,m_Box.h  };
}


void NumberedBox::Render(SDLRenderer* _renderer, Vector2 _WorldPos,bool _drawFilled)
{
	if (_drawFilled)
	{
		_renderer->DrawFilledBox(m_Box, m_Colour, _WorldPos, 1);
	}
	else
	{
		_renderer->DrawBox(m_Box, m_Colour, _WorldPos, 1);
	}
	//_renderer->DrawBox(m_Box, m_Colour, _WorldPos, 1);


	if (m_Text.GetFont() != nullptr)
	{
		m_Text.Render(_renderer, _WorldPos, 2);
	}

}

void NumberedBox::Render(SDLRenderer* _renderer, Vector2 _WorldPos, Vector2 _ScaleMult, bool _drawFilled)
{

	Vector2 zF = _ScaleMult;
	if (_drawFilled)
	{
		_renderer->DrawFilledBox((int)m_Box.pos.x, (int)m_Box.pos.y, (int)m_Box.w*zF.x, (int)m_Box.h*zF.y, m_Colour, _WorldPos, 1);
	}
	else
	{
		Box b = m_Box;
		b.pos *= zF.x;
		b.w *= zF.x;
		b.h *= zF.y;

		_renderer->DrawBox(b, m_Colour, _WorldPos, 1);
	}
	//_renderer->DrawBox(m_Box, m_Colour, _WorldPos, 1);


	if (m_Text.GetFont() != nullptr)
	{
		Vector2 t_Tpos = m_Text.m_pos;
		m_Text.m_pos *= zF.x;
		m_Text.Render(_renderer, _WorldPos, 2);
		m_Text.m_pos = t_Tpos;
	}
}

void NumberedBox::Render(SDLRenderer* _renderer, Vector2 _WorldPos, SDL_Color _col, Vector2 _ScaleMult, bool _drawFilled)
{
	Vector2 zF = _ScaleMult;
	if (_drawFilled)
	{
		_renderer->DrawFilledBox((int)m_Box.pos.x, (int)m_Box.pos.y, (int)m_Box.w * zF.x, (int)m_Box.h * zF.y, _col, _WorldPos, 1);
	}
	else
	{
		Box b = m_Box;
		b.pos *= zF.x;
		b.w *= zF.x;
		b.h *= zF.y;

		_renderer->DrawBox(b, _col, _WorldPos, 1);
	}
	//_renderer->DrawBox(m_Box, m_Colour, _WorldPos, 1);


	if (m_Text.GetFont() != nullptr)
	{
		Vector2 t_Tpos = m_Text.m_pos;
		m_Text.m_pos *= zF.x;
		m_Text.Render(_renderer, _WorldPos, 2);
		m_Text.m_pos = t_Tpos;
	}

}

void NumberedBox::SetBox(Box _box)
{
	m_Box = _box;
	UpdateTextField();
}

void NumberedBox::SetPos(Vector2 _pos)
{
	m_Box.pos = _pos;
	UpdateTextField();
}

void NumberedBox::SetColour(SDL_Color _col)
{
	m_Colour = _col;
	m_Text.SetColour(_col);
}

void NumberedBox::SetFont(TTF_Font* _font)
{
	m_Text.SetFont(_font);
}

void NumberedBox::SetNumber(int _number)
{
	m_Number = _number;
	m_Text.SetText(std::to_string(m_Number));
}
