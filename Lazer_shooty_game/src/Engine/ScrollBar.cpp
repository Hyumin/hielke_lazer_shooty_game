#include "ScrollBar.h"
#include "ButtonBuilder.h"


ScrollBar::ScrollBar()
{
	Init(nullptr);
}

ScrollBar::ScrollBar(Texture* _iconsTexture)
{
	Init(_iconsTexture);
}

ScrollBar::~ScrollBar()
{
	m_Buttons.clear();

}

void ScrollBar::Update(float _dt)
{

}

void ScrollBar::MouseMove(int _x, int _y)
{
	Vector2 prev = m_MousePos;
	m_MousePos = Vector2{ (float)_x,(float)_y };
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x,_y);
	}

	if (m_ScrollBarHeld)
	{
		//place the testoffset based on movement
		Vector2 mouseDelta = m_MousePos - prev;
		//Negative motion
		m_Indicator.pos.y += mouseDelta.y;
		UpdateIndicator();
	
	}
}

void ScrollBar::MouseDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}
	if (_key == 1)
	{
		if (Box::BoxCollision(m_Indicator, m_MousePos))
		{
			m_ScrollBarHeld = true;
		}
	}
}

void ScrollBar::MouseUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}
	if (_key == 1)
	{
		m_ScrollBarHeld = false;
	}
}

void ScrollBar::MouseScroll(int _x, int _y)
{
	if (_y > 0)
	{
		ScrollButtonCallback(-1);
	}
	if (_y < 0)
	{
		ScrollButtonCallback(1);
	}
}

void ScrollBar::Render(SDLRenderer* _renderer)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Render(_renderer);
	}

	SDL_Color indicatorColor = m_NormalColour;
	if (m_ScrollBarHeld)
	{
		indicatorColor =  m_HeldColour;
	}
	_renderer->DrawBox(m_ScrollBox, m_NormalColour, { 0,0 }, 2);
	_renderer->DrawFilledBox(m_Indicator, indicatorColor, { 0,0 },2);

}

float ScrollBar::GetOffset()
{
	return m_Offset;
}

void ScrollBar::SetPosition(Vector2 _pos)
{
	UpdatePosition(_pos);
}

void ScrollBar::SetSize(Vector2 _size)
{
	UpdateSize(_size);
}

void ScrollBar::SetMinimumAndMaximum(float _min, float _max)
{
	m_MinOffset = _min;
	m_MaxOffset = _max;
	SetSize(this->m_Size);
}

void ScrollBar::Init(Texture* _iconsTexture)
{
	std::function<void(int)> func = std::bind(&ScrollBar::ScrollButtonCallback, this, std::placeholders::_1);

	m_ScrollNegativeBtn = ButtonBuilder::BuildButton({ 0,0 }, { 16,16 }, 2, func, -1);
	m_ScrollPositiveBtn = ButtonBuilder::BuildButton({ 0,0 }, { 16,16 }, 2, func, 1);


	if (_iconsTexture != nullptr)
	{
		SDL_Rect norm, clicked, hovered;
		norm = { 0,64,16,16 };
		clicked = { 32,64,16,16 };
		hovered = { 16,64,16,16 };


		m_ScrollNegativeBtn.SetTextureDrawModeWithSheet(_iconsTexture, norm, clicked, hovered);

		norm = { 48,64,16,16 };
		clicked = { 80,64,16,16 };
		hovered = { 64,64,16,16 };
		m_ScrollPositiveBtn.SetTextureDrawModeWithSheet(_iconsTexture, norm, clicked, hovered);

	}
	m_ScrollBox.w = m_ScrollNegativeBtn.GetSize().x;

	m_Buttons.push_back(&m_ScrollPositiveBtn);
	m_Buttons.push_back(&m_ScrollNegativeBtn);
	m_NormalColour = {0,84,0,255};
	m_HeldColour = {0,40,0,255};

	m_ButtonSizePercentages.x = 1;
	m_ButtonSizePercentages.y = 0.1;

	m_Pos = { -1,-1 };
	m_Size = { -1,-1 };
	SetPosition({ 0, 0 });
	SetSize({ 0,0 });
}

void ScrollBar::UpdateIndicator()
{
	float indicposthingy = (m_Indicator.pos.y - m_ScrollBox.pos.y) / (m_ScrollBox.h - m_Indicator.h);
	m_Offset = m_MaxOffset * indicposthingy;
	UpdateOffset();
}

void ScrollBar::UpdateOffset()
{
	if (m_Offset>= m_MaxOffset)
	{
		m_Offset = m_MaxOffset;
	}
	if (m_Offset <= m_MinOffset)
	{
		m_Offset = m_MinOffset;
	}
	float indicposthingy = m_Offset / m_MaxOffset;
	m_Indicator.pos.y = m_ScrollBox.pos.y + (m_ScrollBox.h - m_Indicator.h) * indicposthingy;

	m_OffsetChanged = true;
}

void ScrollBar::UpdatePosition(Vector2 _pos)
{
	Vector2 delta = m_Pos;
	m_Pos = _pos;
	delta = _pos - delta;
	
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		Vector2 newPos = m_Buttons[i]->GetPosition() + delta;
		m_Buttons[i]->SetPosition(newPos);
	}
	m_ScrollBox.pos = m_ScrollNegativeBtn.GetPosition() + Vector2{0,m_ScrollNegativeBtn.GetSize().y};
	m_Indicator.pos.x = m_ScrollBox.pos.x;
	float indicposthingy = m_Offset / m_MaxOffset;
	m_Indicator.pos.y = m_ScrollBox.pos.y + (m_ScrollBox.h - m_Indicator.h) * indicposthingy;
	

}
void ScrollBar::UpdateSize(Vector2 _size)
{
	Vector2 delta = m_Size;
	m_Size = _size;
	delta = _size - delta;

	Vector2 newButtonSize  = m_Size;
	newButtonSize.x *= m_ButtonSizePercentages.x;
	newButtonSize.y *= m_ButtonSizePercentages.y;
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->SetSize(newButtonSize);

	}
	m_ScrollBox.w = m_Size.x * m_ButtonSizePercentages.x;
	m_ScrollBox.h = m_Size.y -newButtonSize.y*2;
	m_Indicator.w = m_ScrollBox.w;
	
	float scaaale = m_Size.y/ (m_MaxOffset+m_Size.y);
	//calculate scrollbar indicator size
	m_Indicator.h = m_ScrollBox.h * scaaale;

	m_ScrollPositiveBtn.SetPosition(m_ScrollBox.pos + Vector2{ 0,m_ScrollBox.h });
	UpdatePosition(this->m_Pos);
}

//The button for the scrolling up and down
void ScrollBar::ScrollButtonCallback(int _direction)
{
	m_Offset += _direction * m_MaxOffset/10;
	UpdateOffset();
}
