#include "AddMapColiderWindow.h"
#include "..\Engine\ButtonBuilder.h"
#include "..\Engine\TextFieldBuilder.h"
#include <string>

AddMapColliderWindow::AddMapColliderWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	m_Map = nullptr;
	Init(_IconsTexture);
}

AddMapColliderWindow::AddMapColliderWindow()
{
	m_Pos = { 0,0 };
	m_Name = "Defaultname";
	m_Map = nullptr;
	Init(nullptr);
}

AddMapColliderWindow::~AddMapColliderWindow()
{
	MapEditorWindow::~MapEditorWindow();
	m_Map = nullptr;
	m_InputTexts.clear();
	
}

void AddMapColliderWindow::Update(float _dt)
{
	MapEditorWindow::Update(_dt);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->Update(_dt);
	}

	//Update textfield texts
	UpdateInputTextFields();

}

void AddMapColliderWindow::MouseDown(unsigned int _key)
{
	MapEditorWindow::MouseDown(_key);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->MouseDown(_key);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}

	if (!Box::BoxCollision(m_ContentBox, m_MousePos)&&!Box::BoxCollision(m_Bar,m_MousePos))
	{

		switch (m_Mode)
		{
		case MapColliderWindowMode::ColiderSelecting:
			if (_key == SDL_BUTTON_LEFT)
			{
				if (Box::BoxCollision(m_CurrentSelectedBox, m_MousePos))
				{
					m_ColliderSelecting = true;

					m_SelectOffset = m_MousePos-m_CurrentSelectedBox.pos;
				}
			}
			break;
		case MapColliderWindowMode::ColliderDragging:

			if (_key == SDL_BUTTON_LEFT)
			{
				m_ColliderDragging = true;
				m_ColliderStart = m_MousePos;
				m_CurrentSelectedBox.pos = m_MousePos;
			}
			break;
		default:
			break;
		}
	}

}

void AddMapColliderWindow::MouseUp(unsigned int _key)
{
	MapEditorWindow::MouseUp(_key);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->MouseUp(_key);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}
	switch (m_Mode)
	{
	case MapColliderWindowMode::ColiderSelecting:

		if (_key == SDL_BUTTON_LEFT)
		{
			m_ColliderSelecting = false;
		}
		break;
	case MapColliderWindowMode::ColliderDragging:

		if (_key == SDL_BUTTON_LEFT)
		{
			m_ColliderDragging = false;
			m_ColliderEnd = m_MousePos;
		}
		break;
	default:
		break;
	}
}

void AddMapColliderWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	MapEditorWindow::MouseMove(_x, _y);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->MouseMove(_x, _y);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x, _y);
	}
	if (m_ColliderDragging)
	{
		m_ColliderEnd = m_MousePos;
		m_CurrentSelectedBox.w = m_ColliderEnd.x - m_ColliderStart.x;
		m_CurrentSelectedBox.h = m_ColliderEnd.y - m_ColliderStart.y;
	}
	if (m_ColliderSelecting)
	{
		m_CurrentSelectedBox.pos = m_MousePos - m_SelectOffset;
	}
}

void AddMapColliderWindow::KeyDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->KeyDown(_key);
	}
}

void AddMapColliderWindow::KeyUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->KeyUp(_key);
	}
}

void AddMapColliderWindow::SetFont(TTF_Font* _font)
{
	MapEditorWindow::SetFont(_font);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->SetFont(_font);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->SetFont(_font);
	}
}

void AddMapColliderWindow::SetMap(Hielke::Map* _map)
{
	m_Map = _map;
}

void AddMapColliderWindow::Render(SDLRenderer* _renderer)
{
	MapEditorWindow::Render(_renderer);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->Render(_renderer);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Render(_renderer);
	}

	_renderer->DrawBox(m_CurrentSelectedBox, { 0xff,0xff,0xff,0xff }, { 0,0 },1);
	_renderer->DrawFilledBox(m_ContentBox,m_Color);
}

void AddMapColliderWindow::Init(Texture* _IconsTexture)
{
	MapEditorWindow::Init(_IconsTexture);
	m_ColliderDragging = false;

	m_ColliderStart = {0,0};
	m_ColliderEnd = { 0,0 };

	m_Mode = MapColliderWindowMode::Default;

	m_AddButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, { 75,25 }, 1, std::bind(&AddMapColliderWindow::CreateBox, this),
		"AddBox", Button::DrawMode::FILLEDRECT, { 0,112,0,255 }, { 10,140,10,255 }, { 0,80,0,255 }, { 0xff,0xff,0xff,0xff });

	m_DragButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, { 75,25 }, 1, std::bind(&AddMapColliderWindow::SetDragMode, this),
		"Drag", Button::DrawMode::FILLEDRECT, { 0,112,0,255 }, { 10,140,10,255 }, { 0,80,0,255 }, { 0xff,0xff,0xff,0xff });
	m_SelectButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, { 75,25 }, 1, std::bind(&AddMapColliderWindow::SetSelectMode, this),
		"Select", Button::DrawMode::FILLEDRECT, { 0,112,0,255 }, { 10,140,10,255 }, { 0,80,0,255 }, { 0xff,0xff,0xff,0xff });

	m_Buttons.push_back(&m_AddButton);
	m_Buttons.push_back(&m_SelectButton);
	m_Buttons.push_back(&m_DragButton);

	//Init input fields
	m_XText = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "X:", nullptr, { 30,5 }, { 50,20 }, 5,
		{ 0,0,0,255 }, { 0,0,0,255 }, { 100,100,100,255 }, { 255,255 ,255 });
	m_YText = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "Y:", nullptr, { 30,5 }, { 50,20 }, 5,
		{ 0,0,0,255 }, { 0,0,0,255 }, { 100,100,100,255 }, { 255,255 ,255 });
	m_WText = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "W:", nullptr, { 30,5 }, { 50,20 }, 5,
		{ 0,0,0,255 }, { 0,0,0,255 }, { 100,100,100,255 }, { 255,255 ,255 });
	m_HText = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "H:", nullptr, { 30,5 }, { 50,20 }, 5,
		{ 0,0,0,255 }, { 0,0,0,255 }, { 100,100,100,255 }, { 255,255 ,255 });

	m_InputTexts.push_back(m_XText);
	m_InputTexts.push_back(m_YText);
	m_InputTexts.push_back(m_WText);
	m_InputTexts.push_back(m_HText);

	//Define the window's limits in terms of size
	m_MaxWidth = 300;
	m_MaxHeight = 120;
	m_MinWidth = 260;

	m_ContentBox.h = (float)m_MaxHeight;
	Reposition();
}

void AddMapColliderWindow::ReScaleContent()
{
	MapEditorWindow::ReScaleContent();
}

void AddMapColliderWindow::Reposition()
{
	MapEditorWindow::Reposition();
	RepositionInputTexts();
	RepositionButtons();//Reposition buttons depnd on input texts therefor has to go second
}

void AddMapColliderWindow::CreateBox()
{
	if (m_Map != nullptr)
	{
		Box b = m_CurrentSelectedBox;
		//check if width or height are negative
		//If so substract it from the position, and then set these values to be positive
		//since no box should have negative values
		if (b.w < 0)
		{
			b.pos.x += b.w;
			b.w *= -1;
		}
		if (b.h < 0)
		{
			b.pos.y += b.h;
			b.h *= -1;
		}
		b.pos /= m_Zoom;
		b.pos += m_WorldPos;
		b.w /= m_Zoom;
		b.h /= m_Zoom;
		m_Map->AddWall(b);
	}
}

void AddMapColliderWindow::SetDragMode()
{
	printf("dragmode enabled \n");
	m_Mode = MapColliderWindowMode::ColliderDragging;
	m_ColliderDragging = false;
}

void AddMapColliderWindow::SetSelectMode()
{
	m_Mode = MapColliderWindowMode::ColiderSelecting;
	m_ColliderSelecting = false;
}

void AddMapColliderWindow::UpdateInputTextFields()
{
	//IF we'er eeither dragging or seleting/moving the box update the text
	//else let the text update the collider
	if (m_ColliderDragging || m_ColliderSelecting)
	{
		m_XText->SetText(std::to_string((int)m_CurrentSelectedBox.pos.x));
		m_YText->SetText(std::to_string((int)m_CurrentSelectedBox.pos.y));
		m_WText->SetText(std::to_string((int)m_CurrentSelectedBox.w));
		m_HText->SetText(std::to_string((int)m_CurrentSelectedBox.h));
	}
	else
	{

		m_CurrentSelectedBox.pos.x = (float)std::stoi(m_XText->GetText());
		m_CurrentSelectedBox.pos.y = (float)std::stoi(m_YText->GetText());
		m_CurrentSelectedBox.w = (float)std::stoi(m_WText->GetText());
		m_CurrentSelectedBox.h = (float)std::stoi(m_HText->GetText());
	
	}

}

void AddMapColliderWindow::RepositionButtons()
{
	Vector2 newPos = m_WText->GetPosition();
	newPos.x = m_ContentBox.pos.x+10;
	newPos.y +=  m_AddButton.GetSize().y;
	m_AddButton.SetPosition(newPos);

	newPos.x += m_AddButton.GetSize().x +10;
	m_DragButton.SetPosition(newPos);

	newPos.x += m_DragButton.GetSize().x+10;
	m_SelectButton.SetPosition(newPos);
}

void AddMapColliderWindow::RepositionInputTexts()
{
	Vector2 offsetX,offsetY,offsetW,offsetH;

	offsetX = { 30,0 };
	offsetY = { 120,0 };
	offsetW = { 30,25 };
	offsetH = { 120,25 };

	m_XText->SetPosition(m_ContentBox.pos + offsetX);
	m_YText->SetPosition(m_ContentBox.pos + offsetY);
	m_WText->SetPosition(m_ContentBox.pos + offsetW);
	m_HText->SetPosition(m_ContentBox.pos + offsetH);
}
