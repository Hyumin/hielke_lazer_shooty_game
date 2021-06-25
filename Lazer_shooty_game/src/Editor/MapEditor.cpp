#include "MapEditor.h"
#include "..\Engine\ButtonBuilder.h"
#include "..\Engine\WindowOpener.h"
using namespace Hielke;

MapEditor::MapEditor()
{
	printf("Default initialization not feasible without resource manager mapeditor.cpp \n");
	m_ResourceManager = nullptr;
	Init();
}

MapEditor::MapEditor(ResourceManager* _resMan)
{
	m_ResourceManager = _resMan;
	Init();
}

MapEditor::~MapEditor()
{
	m_IconsTexture = nullptr;//Texture will be removed in the resource manager
	m_EditorWindows.clear();
}

void MapEditor::Update(float _dt)
{
	Vector2 vel  = { 0,0 };
	if (m_Kup)
	{
		vel.y -= 1;
	}
	if (m_Kright)
	{
		vel.x += 1;
	}
	if (m_Kleft)
	{
		vel.x -= 1;
	}
	if (m_Kdown)
	{
		vel.y += 1;
	}
	if (m_Kshift)
	{
		vel *= m_SprintMultiplier;
	}
	vel *= m_MoveSpeed*m_Zoom;
	m_WorldPos += vel * _dt;

	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Update(_dt);
	}
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->Update(_dt);
		if (m_EditorWindows[i]->m_ReadyForDelete)
		{
			m_EditorWindows.erase(m_EditorWindows.begin() + i);
			continue;
		}
	}

}

void MapEditor::Render(SDLRenderer* _renderer)
{

	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Render(_renderer);
	}
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		//Since adding this to the render function would couse us to rewrite
		// the super base class EditorWindow, I choose put it in the mapeditor window
		//inside of a seperate function instead.
		m_EditorWindows[i]->PassWorldPosAndZoom(m_WorldPos, m_Zoom);
		m_EditorWindows[i]->Render(_renderer);
	}


	if (m_CurrentMap != nullptr)
	{
		m_CurrentMap->RenderZoomed(_renderer, m_WorldPos,m_Zoom);
		if (m_ShowConnectedMap)
		{
			//m_CurrentMap->GetConnectedMap();
			for (unsigned int i = 0; i < m_ConnectedMaps.size(); ++i)
			{
				m_ConnectedMaps[i].map->RenderZoomed(_renderer, m_WorldPos+m_ConnectedMaps[i].offset, m_Zoom);
				_renderer->AddLineZoomed(m_ConnectedMaps[i].startPosLine, m_WorldPos, m_Zoom, 1);
			}

		}
	}
}

void MapEditor::KeyDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->KeyDown(_key);
	}
	switch (_key)
	{
	case SDLK_LEFT:
		m_Kleft = true;
		break;
	case SDLK_DOWN:
		m_Kdown = true;
		break;
	case SDLK_UP:
		m_Kup = true;
		break;
	case SDLK_RIGHT:
		m_Kright = true;
		break;
	case SDLK_LSHIFT:
		m_Kshift = true;
		break;
	}
}

void MapEditor::KeyUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->KeyUp(_key);
	}

	switch (_key)
	{
	case SDLK_LEFT:
		m_Kleft = false;
		break;
	case SDLK_DOWN:
		m_Kdown = false;
		break;
	case SDLK_UP:
		m_Kup = false;
		break;
	case SDLK_RIGHT:
		m_Kright = false;
		break;
	case SDLK_LSHIFT:
		m_Kshift = false;
		break;
	}
}

void MapEditor::MouseUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseUp(_key);
	}
}

void MapEditor::MouseDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseDown(_key);
	}
}

void MapEditor::MouseMove(int _x, int _y)
{

	m_MousePos.x = (float)_x;
	m_MousePos.y = (float)_y;
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x,_y);
		
	}
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseMove(_x, _y);
	}
	CheckIfMouseInAnyWindow();

	UpdateEditorWindowInGUI();

}

void MapEditor::MouseWheel(int _x, int _y)
{
	if (m_HierachyWindow != nullptr)
	{
		m_HierachyWindow->MouseWheel(_x, _y);
	}
}

void MapEditor::ZoomIn()
{
	m_Zoom += 0.1f;
}

void MapEditor::ZoomOut()
{
	if (m_Zoom > 0.1f)
	{
		m_Zoom -= 0.1f;
	}
}

void MapEditor::SaveMap()
{
	printf(" trying to save map but functionality not implented yet :( mapeditor.cpp \n");
	//Exports map

	std::string filepath = WindowOpener::GetFilePathSave("hmap");

	if (filepath != "thisisnotasavelocation")
	{
		m_CurrentMap->SaveMap(filepath);
	}

}

void MapEditor::LoadMap()
{
	//Sets map

	std::string filepath = WindowOpener::GetFilepathOpen("hmap");

	m_CurrentMap = new Map();
	m_CurrentMap = m_ResourceManager->LoadMap(filepath);
	if (m_CurrentMap != nullptr)
	{
		//Reset world pos and zoom after loading 
		m_WorldPos = Vector2{ 0,0 };
		m_Zoom = 1.0f;
		m_CurrentMap->m_DebugMode = true;//Enable debug mode to see the colliders.
		
		for (int i = 0; i < m_EditorWindows.size(); ++i)
		{
			m_EditorWindows[i]->SetMap(m_CurrentMap);
		}
		m_MapBox = Box{0,0, m_CurrentMap->GetBackground()->m_Size.x,m_CurrentMap->GetBackground()->m_Size.y};
		const std::vector<Hielke::ConnectedMap>& conMaps = m_CurrentMap->GetConnectedMaps();
		//Iterate through maps and place the map at their colliders
		m_ConnectedMaps.clear();
		for (unsigned int i = 0; i < conMaps.size(); ++i)
		{
			Hielke::Map* mappie = (m_ResourceManager->GetMap(conMaps[i].fileName));
			if (mappie != nullptr)
			{
				mappie->m_DebugMode = true;
				//Figure out where to place 
				//if the start position is at the far end of the map in terms of X
				// we can assume that the map is to the left of the current map

				MapAndLine mapskie;

				//if we substract the start position from the collider and we get a negative number
				//we should then place the offset either left or upfrom the  original map.
				mapskie.offset = conMaps[i].collider.pos;
				mapskie.offset -= conMaps[i].startPos;

				//normalize to get a direction vector
				mapskie.offset.Normalize();
				Vector2 dir = mapskie.offset;
				//check absolute values and if x is bigger check whether we use
				//negative one or positive one for determining where to place our stuff
				Vector2 result = {};

				if (abs(dir.x)> abs(dir.y))
				{
					if (dir.x <0)
					{
						result.x = mappie->GetBackground()->m_Size.x;
					}
					else
					{
						result.x = m_CurrentMap->GetBackground()->m_Size.x*-1;
					}
					//Since the world pos gets substracted, we need to inverse our result to get the right location
					result.y = conMaps[i].collider.pos.y*-1;
				}
				else 
				{
					if (dir.y < 0)
					{
						result.y = mappie->GetBackground()->m_Size.y;
					}
					else
					{
						result.y = m_CurrentMap->GetBackground()->m_Size.y*-1;
					}
					result.x = conMaps[i].collider.pos.x*-1;
				}


				mapskie.offset = result;

				mapskie.map = mappie;
				mapskie.startPosLine.start = conMaps[i].collider.pos;
				mapskie.startPosLine.end = conMaps[i].startPos;
				mapskie.startPosLine.end -= mapskie.offset;
				mapskie.startPosLine.colour = {0xff,0x00,0x00,0xff};
				m_ConnectedMaps.push_back(mapskie);
			}
		}
		
	}
}

void MapEditor::GiveTopBarBox(Box _topBarBox)
{
	Vector2 currentPos = _topBarBox.pos;
	//will put the buttons according to the size of this box
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		//Adjust height
		m_Buttons[i]->SetPosition(currentPos);
		m_Buttons[i]->SetSize({ m_Buttons[i]->GetSize().x, _topBarBox.h });
		currentPos.x += m_Buttons[i]->GetSize().x;
	}
	m_TopBarBox = _topBarBox;
}

void MapEditor::Init()
{
	try
	{
		GetDefaultAssets();
	}
	catch (std::exception& e)
	{
		printf(e.what());
		return;
	}
	m_WorldPos = { 0,0 };
	m_MoveSpeed = 200.0f;//movement speed is in pixels
	m_SprintMultiplier = 3.0f;

	m_Kdown = false;
	m_Kleft = false;
	m_Kup = false;
	m_Kright = false;
	m_Kshift = false;
	m_ShowConnectedMap = true;

	
	std::string windowName = "Add Collider";
	m_ColliderWindow = new AddMapColliderWindow(Vector2{ 600,300 }, windowName, m_IconsTexture);
	windowName = "Hierachy";
	m_HierachyWindow = new MapHierachyWindow(Vector2{800,200}, windowName, m_IconsTexture);


	m_AddColliderButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, { 150,30 }, 1, std::bind(&MapEditor::CollidorWindowCallback, this),
		"AddCollider", Button::DrawMode::FILLEDRECT, { 0,122,0,255 }, { 10,160,10,255 }, { 0,80,0,255 }, { 0,0,0,255 });

	m_HierachyButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, { 150,30 }, 1, std::bind(&MapEditor::HierachyWindowCallback, this),
		"Hierachy", Button::DrawMode::FILLEDRECT, { 0,122,0,255 }, { 10,160,10,255 }, { 0,80,0,255 }, { 0,0,0,255 });

	if (m_Font != nullptr)
	{
		m_AddColliderButton.SetFont(m_Font);
		m_HierachyButton.SetFont(m_Font);
		m_ColliderWindow->SetFont(m_Font);
		m_HierachyWindow->SetFont(m_Font);
	}

	m_Buttons.push_back(&m_AddColliderButton);
	m_Buttons.push_back(&m_HierachyButton);
	m_EditorWindows.push_back(m_HierachyWindow);//always start hierachy window open
}

void MapEditor::CollidorWindowCallback()
{
	SearchAndActivateWindow(static_cast<MapEditorWindow*>(m_ColliderWindow));
}

void MapEditor::HierachyWindowCallback()
{
	SearchAndActivateWindow(static_cast<MapEditorWindow*>(m_HierachyWindow));
}

void MapEditor::GetDefaultAssets()
{
	if (m_ResourceManager == nullptr)
	{
		throw std::exception("Can't get default assets if there is no resource manager defined \n");
	}
	m_IconsTexture = m_ResourceManager->LoadTexture("Assets//editor//sprite editor icons.png");
	m_Font = m_ResourceManager->LoadFont("Assets//Fonts//LucidaBrightRegular.ttf", 16);
}

void MapEditor::SearchAndActivateWindow(MapEditorWindow* _window)
{
	bool found = false;

	//If the add frame window is not within the 
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		if (m_EditorWindows[i] == _window)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		//set can delete to false;
		_window->m_ReadyForDelete = false;
		_window->MouseMove((unsigned int)m_MousePos.x, (unsigned int)m_MousePos.y);

		_window->SetMap(m_CurrentMap);//Don't forget to set the map after we toggle it on again
		m_EditorWindows.push_back(_window);
		UpdateEditorWindowInGUI();
	}
}

void MapEditor::UpdateEditorWindowInGUI()
{
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->m_MouseInGUI = m_InGUI;
	}
}

void MapEditor::CheckIfMouseInAnyWindow()
{
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		if (m_EditorWindows[i]->m_MouseInWindow)
		{
			m_InGUI = true;
			return;
		}
	}
	if (Box::BoxCollision(m_TopBarBox, m_MousePos))
	{
		m_InGUI = true;
	}
	else
	{
		m_InGUI = false;
	}
}
