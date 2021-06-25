#include "DropDownWidget.h"
#include "ButtonBuilder.h"
#include <functional>

DropDownWidget::DropDownWidget()
{
	m_Pos = { 0,0 };
	m_Size = { 1,1 };
	Init(nullptr);
}

DropDownWidget::DropDownWidget(Vector2 _pos, Vector2 _size, Texture* _icons)
{
	m_Pos = _pos;
	m_Size = _size;
	Init(_icons);
}

DropDownWidget::~DropDownWidget()
{
	delete m_DropDownButton;
	m_Buttons.clear();
}

//We make an assumption both arrays have the same length,
//The strings index correspond with the index of the int vector
void DropDownWidget::SetStates(const std::vector<std::string>& _strings, const std::vector<int>& _values)
{
	size_t size = _strings.size();

	m_CurrentIdentifierButton->SetText(_strings[0]);
	m_CurrentState = 0;
	//Early out when assumption is not met
	if (size != _values.size())
	{
		printf("Assumed the size of both arrays to be equivelant to another, DropDownWidget.cpp \n");
		return;
	}

	//Clear previous arrays
	m_Buttons.clear();
	m_StateBindings.clear();
	std::function<void(int)> func = std::bind(&DropDownWidget::DropDownTextCallback, this, std::placeholders::_1);

	//Generate buttons for these states
	for (uint32_t i = 0; i < size; ++i)
	{
		//Build a button equal with position {0,i*size.y}
		Button* buttonBro = new Button();
		*buttonBro = ButtonBuilder::BuildButtonWireFrameOrFilledRect(Vector2{ 0,(float)(i * size) }, m_Size, m_Layer+1, func,i,
			_strings[i], Button::DrawMode::WIREFRAME, m_Norm, m_Hovered, m_Clicked, m_TextCol);

		if (m_FontRef != nullptr)
		{
			buttonBro->SetFont(m_FontRef);
		}

		m_Buttons.push_back(buttonBro);
	}

	m_StateBindings = _values;
}

void DropDownWidget::Update(float _dt)
{
}

void DropDownWidget::MouseDown(int _key)
{
	m_DropDownButton->MouseDown(_key);
	m_CurrentIdentifierButton->MouseDown(_key);
	if (m_Opened)
	{
		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
		{
			m_Buttons[i]->MouseDown(_key);
		}
	}
}

void DropDownWidget::MouseUp(int _key)
{
	m_DropDownButton->MouseUp(_key);
	m_CurrentIdentifierButton->MouseUp(_key);
	if (m_Opened)
	{
		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
		{
			m_Buttons[i]->MouseUp(_key);
		}
	}
}

void DropDownWidget::MouseMove(int _x, int _y)
{
	m_DropDownButton->MouseMove(_x,_y);
	m_CurrentIdentifierButton->MouseMove(_x,_y);
	if (m_Opened)
	{
		Vector2 relMP = m_Target->m_TargetBox.pos;//Relative mouse pos
		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
		{
			m_Buttons[i]->MouseMove(_x-relMP.x,_y-relMP.y);
		}
	}
}

void DropDownWidget::MouseScroll(int _x, int _y)
{
	//For iff we want to use a scroll bar in the future
}

void DropDownWidget::SetFont(TTF_Font* _font)
{
	m_DropDownButton->SetFont(_font);
	m_CurrentIdentifierButton->SetFont(_font);

	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->SetFont(_font);
	}
}

void DropDownWidget::Render(SDLRenderer* _renderer)
{
	m_DropDownButton->Render(_renderer);
	m_CurrentIdentifierButton->Render(_renderer);
	
	SDL_Color green = { 0,120,0,255 };
	
	Box bruhx = Box{m_Pos.x,m_Pos.y,m_Size.x,m_Size.y};
	_renderer->DrawFilledBox(bruhx, green, { 0,0 },m_Layer);


	if (m_Target == nullptr)
	{
		m_Target =_renderer->CreateRenderTarget(Box{ m_Pos,m_Size.x,m_Size.y * 3 }, m_Layer);
	}
	else
	{
		//Don't render if we're closed
		if (m_Opened)
		{
			FilledBox fb;
			Box& b = m_Target->m_TargetBox;
			SDL_Rect sb = { 0,0,b.w,b.h };
			fb.box = sb;
			fb.col = green;
			m_Target->AddFilledBox(fb);
			for (unsigned int i = 0; i < m_Buttons.size(); ++i)
			{
				m_Buttons[i]->RenderToTarget(_renderer, m_Target, { 0, 0 });
			}
		}
	}

}
void DropDownWidget::SetPosition(const Vector2& _pos)
{
	m_Pos = Vector2{ _pos.x,_pos.y };
	m_CurrentIdentifierButton->SetPosition(m_Pos);
	m_DropDownButton->SetPosition(m_Pos + Vector2{ m_CurrentIdentifierButton->GetSize().x,0 });

	//Position of the buttons depends on the position of the target
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		Vector2 newPos = { 0,0 };
		newPos.y =  i *m_Size.y;
		m_Buttons[i]->SetPosition(newPos);
	}
	if (m_Target != nullptr)
	{
		m_Target->m_TargetBox.pos = m_Pos + Vector2{ 0,m_Size.y };
	}

}

void DropDownWidget::SetSize(const Vector2& _size)
{
	m_Size = _size;

	m_CurrentIdentifierButton->SetSize({ m_Size.x-m_Size.y,m_Size.y });
	m_DropDownButton->SetSize({ m_Size.y,m_Size.y });

	if (m_Target != nullptr)
	{
		m_Target->m_TargetBox.w = m_Size.x;
		m_Target->m_TargetBox.h = m_Size.y * 3;
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->SetSize(m_Size);
	}

}

void DropDownWidget::DropDownTextCallback(int _num)
{
	if (_num >= 0 && _num < m_StateBindings.size())
	{
		m_CurrentState = m_StateBindings[_num];
		m_CurrentIdentifierButton->SetText(m_Buttons[_num]->GetText());
		m_Opened = false;
	}
}

void DropDownWidget::Init(Texture* _icons)
{
	m_IconsTexture = _icons;

	//Init dropdown button and currentidentifier button
	std::function<void()> func = std::bind(&DropDownWidget::DropDownCallback, this);
	m_Norm = { 0,0,0,255 };
	m_Hovered = { 100,100,100,255 };
	m_Clicked = { 255,255,255,255 };
	m_TextCol = { 255,255,255,255 };

	m_DropDownButton = new Button(ButtonBuilder::BuildButton({ 0,0 }, { 0,0 },3,func));
	m_CurrentIdentifierButton = new Button(ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, { 0,0 },3,func," ",Button::DrawMode::WIREFRAME,
		m_Norm,m_Hovered,m_Clicked, m_TextCol));

	m_CurrentIdentifierButton->SetWireFrameMode(m_Norm,m_Hovered,m_Clicked);
	SDL_Rect click, hover, norm;
	norm = {48,64,16,16};
	hover = {64,64,16,16};
	click = {80,64,16,16};
	m_DropDownButton->SetTextureDrawModeWithSheet(_icons, norm, click, hover);

	m_Layer = 3;
}

void DropDownWidget::DropDownCallback()
{
	m_Opened = m_Opened ? false : true;
}
