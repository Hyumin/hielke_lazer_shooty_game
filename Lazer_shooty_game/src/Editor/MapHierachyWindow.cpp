#include "MapHierachyWindow.h"
#include "..\Engine\ButtonBuilder.h"
#include "..\Engine\TextFieldBuilder.h"
#include "..\Engine\Texture.h"
#include <string>

MapHierachyWindow::MapHierachyWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	m_Map = nullptr;
	Init(_IconsTexture);
}

MapHierachyWindow::MapHierachyWindow()
{
	m_Pos = { 0,0 };
	m_Name = "Defaultname";
	m_Map = nullptr;
	Init(nullptr);
}

MapHierachyWindow::~MapHierachyWindow()
{
	MapEditorWindow::~MapEditorWindow();
	m_Map = nullptr;
	m_InputTexts.clear();
	m_Buttons.clear();
	delete m_ScrollBarObj;
	m_WallButtons.clear();
	delete m_DropDownWidget;
}

void MapHierachyWindow::Update(float _dt)
{
	MapEditorWindow::Update(_dt);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->Update(_dt);
	}
	if (m_Map != nullptr)
	{
		if (m_Map->GetColliders().size() != m_WallButtons.size())
		{
			GenerateHierachyWalls();
		}
	}


	switch (m_Mode)
	{
	case MapHierachyWindowMode::WallSelect:
		if (m_ScrollBarObj->m_OffsetChanged)
		{
			UpdateHierachyButtonArray(&m_WallButtons);
		}
	break;
	case MapHierachyWindowMode::Enemy:
		if (m_ScrollBarObj->m_OffsetChanged)
		{
			UpdateHierachyButtonArray(&m_EnemyButtons);
		}
		break;
	case MapHierachyWindowMode::ConnectedMapMode:
		if (m_ScrollBarObj->m_OffsetChanged)
		{
			UpdateHierachyButtonArray(&m_ConnectedMapButtons);
		}
		break;
	default:
		break;
	}

}

void MapHierachyWindow::MouseDown(unsigned int _key)
{
	MapEditorWindow::MouseDown(_key);
	m_ScrollBarObj->MouseDown(_key);
	m_DropDownWidget->MouseDown(_key);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->MouseDown(_key);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}
	switch (m_Mode)
	{
	case MapHierachyWindowMode::WallSelect:
	{
		for (unsigned int i = 0; i < m_WallButtons.size(); ++i)
		{
			m_WallButtons[i]->MouseDown(_key);
			if (_key == 1 && !m_MouseInGUI)
			{
				if (Box::BoxCollision(m_Map->GetColliders()[i], m_MousePos + m_WorldPos) )
				{

					m_CurrentSelectedWall = &m_Map->GetColliders()[i];
					m_SelectedIndex = i;
				}
			}
		}
	}
	break;
	case MapHierachyWindowMode::Enemy:
		for (unsigned int i = 0; i < m_EnemyButtons.size(); ++i)
		{
			m_EnemyButtons[i]->MouseDown(_key);
		}

		break;
	case MapHierachyWindowMode::ConnectedMapMode:
		for (unsigned int i = 0; i < m_ConnectedMapButtons.size(); ++i)
		{
			m_ConnectedMapButtons[i]->MouseDown(_key);
		}
		break;
	default:
		break;
	}
	

}

void MapHierachyWindow::MouseUp(unsigned int _key)
{
	MapEditorWindow::MouseUp(_key);
	m_ScrollBarObj->MouseUp(_key);
	m_DropDownWidget->MouseUp(_key);

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
	case MapHierachyWindowMode::WallSelect:
	{
		for (unsigned int i = 0; i < m_WallButtons.size(); ++i)
		{
			m_WallButtons[i]->MouseUp(_key);
		}
	}
	break;
	case MapHierachyWindowMode::Enemy:
		for (unsigned int i = 0; i < m_EnemyButtons.size(); ++i)
		{
			m_EnemyButtons[i]->MouseUp(_key);
		}
		
		break;
	case MapHierachyWindowMode::ConnectedMapMode:
		for (unsigned int i = 0; i < m_ConnectedMapButtons.size(); ++i)
		{
			m_ConnectedMapButtons[i]->MouseUp(_key);
		}
		break;
	default:
		break;
	}

	//Selecting thing ins the world
	switch (m_Mode)
	{
	case MapHierachyWindowMode::WallSelect:

		break;
	case MapHierachyWindowMode::Enemy:


		break;
	default:
		break;
	}
}

void MapHierachyWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	Vector2 prev = m_MousePos;
	MapEditorWindow::MouseMove(_x, _y);
	m_ScrollBarObj->MouseMove(_x, _y);


	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->MouseMove(_x, _y);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x, _y);
	}
	m_DropDownWidget->MouseMove(_x, _y);
	int relativeX = (int)m_Target->m_TargetBox.pos.x;
	int relativey = (int)m_Target->m_TargetBox.pos.y;
	switch (m_Mode)
	{
	case MapHierachyWindowMode::WallSelect:
	{
		for (unsigned int i  =0; i<m_WallButtons.size(); ++i)
		{
			m_WallButtons[i]->MouseMove(_x-relativeX, _y-relativey);
		}
	}
	break;
	case MapHierachyWindowMode::Enemy:
	{
		for (unsigned int i = 0; i < m_EnemyButtons.size(); ++i)
		{
			m_EnemyButtons[i]->MouseMove(_x - relativeX, _y - relativey);
		}
	}
		break;
	case MapHierachyWindowMode::ConnectedMapMode:
	{
		for (unsigned int i = 0; i < m_ConnectedMapButtons.size(); ++i)
		{
			m_ConnectedMapButtons[i]->MouseMove(_x - relativeX, _y - relativey);
		}
	}
		break;
	default:
		break;
	}
}

void MapHierachyWindow::KeyDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->KeyDown(_key);
	}
}

void MapHierachyWindow::KeyUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->KeyUp(_key);
	}

}

void MapHierachyWindow::SetFont(TTF_Font* _font)
{
	MapEditorWindow::SetFont(_font);
	m_Font = _font;
	m_DropDownWidget->SetFont(_font);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->SetFont(_font);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->SetFont(_font);
	}
}

void MapHierachyWindow::SetMap(Hielke::Map* _map)
{
	m_Map = _map;
	switch (m_Mode)
	{
	case MapHierachyWindowMode::WallSelect:
		GenerateHierachyWalls();
		break;
	case MapHierachyWindowMode::Enemy:
		GenerateEnemyButtons();
		break;
	case MapHierachyWindowMode::ConnectedMapMode:
		GenerateConnectedMapButtons();
		break;
	}
}

void MapHierachyWindow::MouseWheel(int _x, int _y)
{
	m_ScrollBarObj->MouseScroll(_x, _y);
	m_DropDownWidget->MouseScroll(_x, _y);
}

void MapHierachyWindow::Render(SDLRenderer* _renderer)
{
		MapEditorWindow::Render(_renderer);

		if (m_Target == nullptr)
		{
			m_Target = _renderer->CreateRenderTarget(m_ContentBox, m_Layer+1);
		}

		for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
		{
			m_InputTexts[i]->Render(_renderer);
		}
		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
		{
			m_Buttons[i]->Render(_renderer);
		}
		_renderer->DrawFilledBox(m_ContentBox, m_Color, { 0,0 },m_Layer);
		m_DropDownWidget->Render(_renderer);

		m_ScrollBarObj->Render(_renderer);
		//Depending on the selection mode we want to visualize currently selected
		// thing with a white box
		SDL_Color selectedCol = { 0xff,0xff,0xff,0xff };
		if (m_Map != nullptr)
		{
		switch (m_Mode)
		{
		case MapHierachyWindowMode::WallSelect:
		{
			if (m_CurrentSelectedWall != nullptr)
			{
				Box b = *m_CurrentSelectedWall;
				_renderer->DrawBoxZoomed(b, selectedCol, m_WorldPos, m_Zoom, m_Layer);
			}
			if (m_Target != nullptr)
			{
				//Render ze buttons
				for (unsigned int i = 0; i < m_WallButtons.size(); ++i)
				{
					m_WallButtons[i]->RenderToTarget(_renderer, m_Target, { 0,0 });
				}
				if (m_SelectedIndex >= 0)
				{
					//Add wireframe renderino here

					WireFrameBox tb;
					tb.box = m_WallButtons[m_SelectedIndex]->GetCollider();
					tb.col = {255,255,255,255};
					tb.thickness = 1;
					m_Target->AddBox(tb);
				}
			}
		}
		break;
		case MapHierachyWindowMode::Enemy:
			if (m_CurrentSelectedEnemy != nullptr)
			{
			}
			if (m_Target != nullptr)
			{
				//Render ze buttons
				for (unsigned int i = 0; i < m_EnemyButtons.size(); ++i)
				{
					m_EnemyButtons[i]->RenderToTarget(_renderer, m_Target, { 0,0 });
				}
				if (m_SelectedIndex >= 0)
				{
					//Add wireframe renderino here

					WireFrameBox tb;
					tb.box = m_EnemyButtons[m_SelectedIndex]->GetCollider();
					tb.col = { 255,255,255,255 };
					tb.thickness = 1;
					m_Target->AddBox(tb);
				}
			}

			break;
		case MapHierachyWindowMode::ConnectedMapMode:
			
			if (m_CurrentlySelectedConnectedMap != nullptr)
			{
				_renderer->DrawBoxZoomed(m_CurrentlySelectedConnectedMap->collider,selectedCol, m_WorldPos, m_Zoom, m_Layer);
			}
			if (m_Target != nullptr)
			{
				//Render ze buttons
				for (unsigned int i = 0; i < m_ConnectedMapButtons.size(); ++i)
				{
					m_ConnectedMapButtons[i]->RenderToTarget(_renderer, m_Target, { 0,0 });
				}
				if (m_SelectedIndex >= 0)
				{
					//Add wireframe renderino here

					WireFrameBox tb;
					tb.box = m_ConnectedMapButtons[m_SelectedIndex]->GetCollider();
					tb.col = { 255,255,255,255 };
					tb.thickness = 1;
					m_Target->AddBox(tb);
				}
			}
			break;
		default:
			break;
		}
	}
}



void MapHierachyWindow::Init(Texture* _IconsTexture)
{
	MapEditorWindow::Init(_IconsTexture);

	m_CurrentSelectedEnemy = nullptr;
	m_CurrentSelectedWall = nullptr;

	//Define the window's limits in terms of size
	m_MaxWidth = 500;
	m_MaxHeight = 800;
	m_MinWidth = 260;

	m_ContentBox.h = (float)m_MaxHeight / 2;
	m_PrevContentBox = m_ContentBox;

	m_ScrollBarObj = new ScrollBar{ m_IconTexture };
	m_ScrollBarObj->SetPosition(m_ContentBox.pos);
	m_ScrollBarObj->SetSize({m_ContentBox.w, m_ContentBox.h});

	m_Target = nullptr;
	m_MouseOverWindow = false;
	m_SelectedIndex = -1;
	m_DropDownWidget = new DropDownWidget(m_ContentBox.pos, Vector2{ m_ContentBox.w,40 }, _IconsTexture);
	std::vector<std::string> strings = { "ConncetedMapMode","WallSelect","NPC","Enemy" };
	std::vector<int> enumValues = { (int)MapHierachyWindowMode::ConnectedMapMode,(int)MapHierachyWindowMode::WallSelect,(int)MapHierachyWindowMode::NPC,(int)MapHierachyWindowMode::Enemy };

	m_DropDownWidget->SetStates(strings, enumValues);

	Reposition();

	SetWallMode();

	m_DropDownWidget->SetSize({ m_ContentBox.w,50 });
	



}

void MapHierachyWindow::ReScaleContent()
{
	MapEditorWindow::ReScaleContent();
	m_DropDownWidget->SetSize({ m_ContentBox.w,30 });
	if (m_Target != nullptr)
	{
		Box berhx = m_ContentBox;
		berhx.h -= m_DropDownWidget->GetSize().y;
		m_Target->CreateTexture(berhx);//Every tune we rescale we recreate the texture
	}
}

void MapHierachyWindow::Reposition()
{
	m_PrevContentBox = m_ContentBox;
	MapEditorWindow::Reposition();
	RepositionInputTexts();
	RepositionButtons();//Reposition buttons depnd on input texts therefor has to go second
	if (m_Target != nullptr)
	{
		m_Target->m_TargetBox.pos = m_ContentBox.pos + Vector2{0,-m_DropDownWidget->GetSize().y};
	}
	float scrollBarWidth = m_ContentBox.w / 10;
	m_ScrollBarObj->SetPosition(m_ContentBox.pos + Vector2{ m_ContentBox.w- scrollBarWidth,0 });
	m_ScrollBarObj->SetSize(Vector2{ scrollBarWidth, m_ContentBox.h -m_ContentScaleObject.m_Size.y});

	m_DropDownWidget->SetPosition(m_ContentBox.pos);
}


void MapHierachyWindow::RepositionButtons()
{
	

}


void MapHierachyWindow::RepositionInputTexts()
{

}

void MapHierachyWindow::SelectWall(int _index)
{
	if (m_Map != nullptr)
	{
		m_CurrentSelectedWall = &m_Map->GetColliders()[_index];
		m_SelectedIndex = _index;
	}
}

void MapHierachyWindow::SelectEnemy(int _index)
{
	if (m_Map != nullptr)
	{
		m_CurrentSelectedEnemy = m_Map->GetEnemies()[_index];
		m_SelectedIndex = _index;
	}
}

void MapHierachyWindow::SelectConnectedMap(int _index)
{
	if(m_Map != nullptr)
	{
		m_CurrentlySelectedConnectedMap = &m_Map->GetConnectedMaps()[_index];
		m_SelectedIndex = _index;
	}
}

void MapHierachyWindow::SetEnemyMode()
{
	m_Mode = MapHierachyWindowMode::Enemy;
	m_SelectedIndex = -1;

	//Generate enemybuttons
	GenerateEnemyButtons();

}

void MapHierachyWindow::SetWallMode()
{
	m_Mode = MapHierachyWindowMode::WallSelect;
	m_SelectedIndex = -1;

	//Generate enemybuttons
	GenerateHierachyWalls();
}

void MapHierachyWindow::SetConnectedMapMode()
{
	m_Mode = MapHierachyWindowMode::ConnectedMapMode;
	m_SelectedIndex = -1;

	//Generate enemybuttons
	GenerateConnectedMapButtons();
}

void MapHierachyWindow::GenerateButtons(unsigned int _num, int _size, std::string _name, std::function<void(int)> _func,std::vector<Button*>* _outputVector)
{
	_outputVector->clear();
	Vector2 pos = { 0,0 };
	Vector2 size = {100,(float)_size };
	for (unsigned int i = 0; i < _num; ++i)
	{
		pos.y = i * _size;//hardcoded for now
		std::string name = _name + std::to_string(i);

		Button* b = new Button(ButtonBuilder::BuildButtonWireFrameOrFilledRect(pos, size, 2, _func, (int)i, name, Button::DrawMode::FILLEDRECT,
			m_Color, m_LightColor, m_DarkColor, SDL_Color{ 0xff,0xff,0xff,0xff }));

		b->SetFont(m_Font);

		_outputVector->push_back(b);
	}
	float m_MaxOffset = (_num * _size) - m_ContentBox.h;
	m_ScrollBarObj->SetMinimumAndMaximum(0, m_MaxOffset);
}

void MapHierachyWindow::GenerateHierachyWalls()
{
	
	if (m_Map != nullptr)
	{
		std::vector<Box> walls = m_Map->GetColliders();
		std::function<void(int)> func= std::bind(&MapHierachyWindow::SelectWall, this, std::placeholders::_1);
		GenerateButtons((unsigned int)walls.size(), 35, "Wall", func, &m_WallButtons);	
	}
}

void MapHierachyWindow::GenerateEnemyButtons()
{
	if (m_Map != nullptr)
	{
		std::vector<Enemy*> enemies = m_Map->GetEnemies();
		std::function<void(int)> func = std::bind(&MapHierachyWindow::SelectEnemy, this, std::placeholders::_1);
		GenerateButtons((unsigned int)enemies.size(), 35, "Enemy", func, &m_EnemyButtons);
	}
}

void MapHierachyWindow::GenerateConnectedMapButtons()
{
	if (m_Map != nullptr)
	{
		std::vector<Hielke::ConnectedMap> maps = m_Map->GetConnectedMaps();
		std::function<void(int)> func = std::bind(&MapHierachyWindow::SelectConnectedMap, this, std::placeholders::_1);
		GenerateButtons((unsigned int)maps.size(), 35, "ConMap", func, &m_ConnectedMapButtons);
	}
}

void MapHierachyWindow::UpdateHierachyButtonArray(std::vector<Button*>* _array)
{
	for (unsigned int i = 0; i < _array->size(); ++i)
	{
		Vector2 newPos = _array[0][i]->GetPosition();
		newPos.y = i * _array[0][i]->GetSize().y - m_ScrollBarObj->GetOffset();
		_array[0][i]->SetPosition(newPos);
	}
	m_ScrollBarObj->m_OffsetChanged = false;
}
