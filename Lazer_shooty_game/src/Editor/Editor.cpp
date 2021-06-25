#include "Editor.h"
#include "..\Engine\ButtonBuilder.h"

Editor::Editor(ResourceManager* _resman)
{
	Init(_resman);
}

Editor::~Editor()
{
	delete m_AnimClipEditor;
	m_AnimClipEditor = nullptr;
	delete m_MapEditor;
	m_MapEditor = nullptr;
	m_EditorIconsTexture = nullptr;
	m_ResourceManager = nullptr;
}

void Editor::KeyDown(unsigned int _key)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->KeyDown(_key);
		break;
	case EditorState::Map:
		m_MapEditor->KeyDown(_key);
		break;
	}

}

void Editor::KeyUp(unsigned int _key)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->KeyUp(_key);
		break;
	case EditorState::Map:
		m_MapEditor->KeyUp(_key);
		break;
	}
	switch (_key)
	{
	case SDLK_KP_PLUS:
		ZoomInCallback();
		break;
	case SDLK_KP_MINUS:
		ZoomOutCallback();
		break;
	default:
		break;
	}
}

void Editor::MouseDown(unsigned int _key)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->MouseDown(_key);
		break;
	case EditorState::Map:
		m_MapEditor->MouseDown(_key);
		break;
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}
}

void Editor::MouseUp(unsigned int _key)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->MouseUp(_key);
		
		break;
	case EditorState::Map:
		m_MapEditor->MouseUp(_key);
		break;
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}
}

void Editor::MouseMove(int _x, int _y)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->MouseMove(_x, _y);
		break;
	case EditorState::Map:
		m_MapEditor->MouseMove(_x, _y);
		break;
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x,_y);
	}
}

void Editor::MouseWheel(int _x, int _y)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->MouseWheel(_x,_y);
		break;
	case EditorState::Map:
		m_MapEditor->MouseWheel(_x,_y);
		break;
	}
}

void Editor::Update(float _dt)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->Update(_dt);
		break;
	case EditorState::Map:
		m_MapEditor->Update(_dt);
		break;
	}
}

void Editor::Render(SDLRenderer* _renderer)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->Render(_renderer);
		break;
	case EditorState::Map:
		m_MapEditor->Render(_renderer);
		break;
	}
	//Render default buttons and topbar
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Render(_renderer);
	}
	_renderer->DrawFilledBox(m_TopBar, m_BarColour, { 0,0 },1);
}

void Editor::OnWindowResize(unsigned int _width, unsigned int _height)
{
	m_WindowWidth = _width;
	m_WindowHeight = _height;

	m_TopBar.w = (float)m_WindowWidth;
	m_TopBar.h = (float)_height / 20;

	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->SetSize(Vector2{ m_TopBar.h, m_TopBar.h });
	}
	//Since we don't want the switch mode button's size to be rectangle we perform
	// a seperate operation here
	m_SwitchModeButton.SetSize({ m_TopBar.h*3,m_TopBar.h });

	m_SaveButton.SetPosition({m_TopBar.h,0});
	m_ZoomOutButton.SetPosition({m_TopBar.w-m_TopBar.h,0});
	m_ZoomInButton.SetPosition({ m_TopBar.w - m_TopBar.h*2,0});
	m_SwitchModeButton.SetPosition(m_ZoomInButton.GetPosition() - Vector2{m_SwitchModeButton.GetSize().x, 0});;
	
	Box availableAreaForSpecificEditors = m_TopBar;

	availableAreaForSpecificEditors.pos.x = m_SaveButton.GetPosition().x + m_SaveButton.GetSize().x;
	availableAreaForSpecificEditors.w -=  m_ZoomInButton.GetSize().x+ m_ZoomOutButton.GetSize().x+ m_SwitchModeButton.GetSize().x;
	m_AnimClipEditor->GiveTopBarBox(availableAreaForSpecificEditors);
	m_MapEditor->GiveTopBarBox(availableAreaForSpecificEditors);
}

void Editor::Init(ResourceManager* _resman)
{
	m_ResourceManager = _resman;
	m_AnimClipEditor = new AnimationClipEditor(m_ResourceManager);
	m_MapEditor = new MapEditor(m_ResourceManager);
	m_EditorState = EditorState::AnimationClip;
	m_BarColour = { 0,122,0,255 };
	LoadAssets();

	m_TopBar = {0,0,0,0};

	m_LoadButton = ButtonBuilder::BuildButton({ 0,0 }, { 25,25 }, 1, std::bind(&Editor::LoadCallback, this));
	m_SaveButton = ButtonBuilder::BuildButton({ 25,0 }, { 25,25 }, 1, std::bind(&Editor::SaveCallback, this));
	m_ZoomInButton = ButtonBuilder::BuildButton({ 1280 - 50,0 }, { 25,25 }, 1, std::bind(&Editor::ZoomInCallback, this));
	m_ZoomOutButton = ButtonBuilder::BuildButton({ 1280 - 25,0 }, { 25,25 }, 1, std::bind(&Editor::ZoomOutCallback, this));
	m_SwitchModeButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({m_ZoomInButton.GetPosition().x -m_ZoomInButton.GetSize().x-150,0},
		{ 150,25 }, 1, std::bind(&Editor::SwitchModeCallBack, this), "SwitchMode", Button::DrawMode::FILLEDRECT, m_BarColour, { 10,160,10,255 }, { 0,90,0,255 }, {0,0,0,255});

	m_SwitchModeButton.SetFont(m_Font);
	//Initialize buttons
	if (m_EditorIconsTexture != nullptr)
	{
		SDL_Rect norm = { 96,16,16,16 };
		SDL_Rect clicked = { 128,16,16,16 };
		SDL_Rect hover = { 112,16,16,16 };

		m_SaveButton.SetTextureDrawModeWithSheet(m_EditorIconsTexture, norm, clicked, hover);
		norm = { 96,32,16,16 };
		clicked = { 128,32,16,16 };
		hover = { 112,32,16,16 };
		m_LoadButton.SetTextureDrawModeWithSheet(m_EditorIconsTexture, norm, clicked, hover);

		norm = { 48,48,16,16 };
		clicked = { 80,48,16,16 };
		hover = { 64,48,16,16 };
		m_ZoomInButton.SetTextureDrawModeWithSheet(m_EditorIconsTexture, norm, clicked, hover);
		norm = { 96,48,16,16 };
		clicked = { 128,48,16,16 };
		hover = { 112,48,16,16 };
		m_ZoomOutButton.SetTextureDrawModeWithSheet(m_EditorIconsTexture, norm, clicked, hover);

		m_Buttons.push_back(&m_ZoomInButton);
		m_Buttons.push_back(&m_ZoomOutButton);
		m_Buttons.push_back(&m_LoadButton);
		m_Buttons.push_back(&m_SaveButton);
	}
	m_Buttons.push_back(&m_SwitchModeButton);
	//Initialize buttons etc.

}

void Editor::LoadAssets()
{
	if (m_ResourceManager == nullptr)
	{
		throw std::exception("Resource manager was not initialized, can't load icontexture | Editor.cpp \n");
	}
	m_EditorIconsTexture = m_ResourceManager->LoadTexture("Assets//editor//sprite editor icons.png");
	m_Font = m_ResourceManager->LoadFont("Assets//Fonts//LucidaBrightRegular.ttf", 16);
}

void Editor::SaveCallback()
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->SaveClip();
		break;
	case EditorState::Map:
		m_MapEditor->SaveMap();
		break;
	}
}

void Editor::LoadCallback()
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->LoadClip();
		break;
	case EditorState::Map:
		m_MapEditor->LoadMap();
		break;
	}
}

void Editor::ZoomInCallback()
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->ZoomIn();
		break;
	case EditorState::Map:
		m_MapEditor->ZoomIn();
		break;
	}
}

void Editor::ZoomOutCallback()
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->ZoomOut();
		break;
	case EditorState::Map:
		m_MapEditor->ZoomOut();
		break;
	}
}

void Editor::SwitchModeCallBack()
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_EditorState = EditorState::Map;
		break;
	case EditorState::Map:
		m_EditorState = EditorState::AnimationClip;
		break;
	default:
		break;
	}
}
