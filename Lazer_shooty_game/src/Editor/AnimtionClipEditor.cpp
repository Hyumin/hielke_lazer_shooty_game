#include "AnimtionClipEditor.h"

#include "EditorWindow.h"
#include "AnimClipEditorWindow.h"
#include "..\Engine\TextField.h"
#include "AnimationWindow.h"
#include "AddFrameWindow.h"
#include "..\Engine\ButtonBuilder.h"

AnimationClipEditor::AnimationClipEditor()
{
	m_ResMan = NULL;
	Init();
}

AnimationClipEditor::AnimationClipEditor(ResourceManager* _resMan)
{
	m_ResMan = _resMan;
	Init();
}

AnimationClipEditor::~AnimationClipEditor()
{
	delete m_AnimationWindow;
	m_AnimationWindow = nullptr;
	delete m_AddFrameWindow;
	m_AddFrameWindow = nullptr;
}

void AnimationClipEditor::Init()
{
	//Load default resources
	LoadDefaultAssets();

	m_MousePos = Vector2(0, 0);
	m_Position = Vector2(0, 0);
	m_ClipName = "newClip";
	m_FileName = "newClip.hanimclip";
	std::string windowName = "Animation Window";

	AnimationWindow* animWindow = new AnimationWindow(Vector2{ 700,100 }, windowName, m_EditorIconsTexture);
	AddFrameWindow* frameWin = new AddFrameWindow(Vector2{ 300,300 }, std::string("AddFrame window"), m_EditorIconsTexture);
	m_AddFrameWindow = frameWin;
	
	m_NewFileButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 50,0 }, { 100,25 }, 1, std::bind(&AnimationClipEditor::CreateNewFile, this),
		"New clip", Button::DrawMode::FILLEDRECT, { 0,140,15,255 }, { 0,160,15, 255 }, { 0,235,15,255 }, { 0,0,0,255 });

	m_OpenAddFrameWindow = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 150,0 }, { 100,25 }, 1, std::bind(&AnimationClipEditor::OpenAddFrameWindow, this),
		"AddFrame", Button::DrawMode::FILLEDRECT, { 0,140,15,255 }, { 0,160,15, 255 }, { 0,235,15,255 }, {0,0,0,255});

	m_OpenAnimationWindow = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 250,0 }, { 100,25 }, 1, std::bind(&AnimationClipEditor::OpenAnimationWindow, this),
		"Animation", Button::DrawMode::FILLEDRECT, { 0,140,15,255 }, { 0,160,15, 255 }, { 0,235,15,255 }, { 0,0,0,255 });

	SDL_Rect norm = {96,16,16,16};
	SDL_Rect clicked = {128,16,16,16};
	SDL_Rect hover = {112,16,16,16};

	Object obj = Object{};


	m_AnimationWindow = animWindow;
	m_EditorWindows.push_back(m_AddFrameWindow);
	m_EditorWindows.push_back(m_AnimationWindow);


	if (m_DefaultFont != nullptr)
	{
		m_OpenAddFrameWindow.SetFont(m_DefaultFont);
		m_OpenAnimationWindow.SetFont(m_DefaultFont);
		m_NewFileButton.SetFont(m_DefaultFont);
		m_AnimationWindow->SetFont(m_DefaultFont);
		m_AddFrameWindow->SetFont(m_DefaultFont);
	}


	m_Buttons.push_back(&m_OpenAddFrameWindow);
	m_Buttons.push_back(&m_OpenAnimationWindow);
	m_Buttons.push_back(&m_NewFileButton);

	m_HighlightedBoxColour = {255,0,0,255};
}

void AnimationClipEditor::KeyDown(unsigned int _key)
{

	switch (_key)
	{
	case SDLK_UP:
		m_Up = true;
		break;
	case SDLK_DOWN:
		m_Down = true;
		break;
	case SDLK_RIGHT:
		m_Right = true;
		break;
	case SDLK_LEFT:
		m_Left = true;
		break;
	case SDLK_LSHIFT:
		m_Sprinting = true;
		break;
	}
	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->KeyDown(_key);
	}
}

void AnimationClipEditor::KeyUp(unsigned int _key)
{
	switch (_key)
	{
	case SDLK_UP:
		m_Up = false;
		break;
	case SDLK_DOWN:
		m_Down = false;
		break;
	case SDLK_RIGHT:
		m_Right = false;
		break;
	case SDLK_LEFT:
		m_Left = false;
		break;
	case SDLK_LSHIFT:
		m_Sprinting = false;
		break;
	}

	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->KeyUp(_key);
	}
}

void AnimationClipEditor::MouseDown(unsigned int _key)
{
	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseDown(_key);
	}
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}

}

void AnimationClipEditor::MouseUp(unsigned int _key)
{
	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseUp(_key);
	}
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}

}

void AnimationClipEditor::MouseMove(int _x, int _y)
{
	m_MousePos = Vector2((float)_x,(float) _y);
	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseMove(_x,_y);
	}
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x,_y);
	}

}

void AnimationClipEditor::MouseWheel(int _x, int _y)
{
}

void AnimationClipEditor::GenerateNumberedBoxes()
{
	m_NumbrdBoxes.clear();
	if (m_CurrentClip.m_ClipName != " ")
	{
		//Generate numbered boxes based on the current clips rects
		for (unsigned int i = 0; i < m_CurrentClip.m_SourceRects.size(); ++i)
		{
			NumberedBox nb = NumberedBox(i);
			Box box = Box{};

			const SDL_Rect& rect = m_CurrentClip.m_SourceRects[i];
			box.pos = Vector2{ (float)rect.x,(float)rect.y };
			box.w = (float)rect.w;
			box.h = (float)rect.h;

			nb.SetBox(box);
			nb.SetColour({200,200,200,255});
			nb.SetFont(m_DefaultFont);
			m_NumbrdBoxes.push_back(nb);
		}
	}
}
void AnimationClipEditor::Update(float _dt)
{
	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->Update(_dt);
		if (m_EditorWindows[i]->m_ReadyForDelete)
		{
			m_EditorWindows.erase(m_EditorWindows.begin() + i);
			continue;
		}
		if (m_EditorWindows[i]->m_ChangeToAnimationClip)
		{
			GenerateNumberedBoxes();
			m_EditorWindows[i]->m_ChangeToAnimationClip = false;
		}
	}

	//Figure out velocity
	Vector2 direction = Vector2{ 0,0 };
	float sprintMod = m_Sprinting ? m_SprintMultiplier : 1;
	if (m_Up)
	{
		direction.y -= 1;
	}
	if (m_Left)
	{
		direction.x -= 1;
	}
	if (m_Right)
	{
		direction.x += 1;
	}
	if (m_Down)
	{
		direction.y += 1;
	}

	m_Position += direction * m_Speed * _dt*m_Zoom*sprintMod;
	m_AddFrameWindow->m_ViewPos = m_Position;
	m_AddFrameWindow->m_ZoomVector = {m_Zoom,m_Zoom};

}

void AnimationClipEditor::SetResourceManager(ResourceManager* _resMan)
{
	m_ResMan = _resMan;
	LoadDefaultAssets();
}


void AnimationClipEditor::Render(SDLRenderer* _renderer)
{
	Vector2 zoomVector = {m_Zoom,m_Zoom};
	m_SpriteSheet.Render(_renderer, m_Position,zoomVector,0);
	_renderer->DrawFilledBox(0, 0, 1280, 25, SDL_Color{ 0,122,0,255 });


	for (unsigned int i = 0; i < m_NumbrdBoxes.size(); ++i)
	{
		if(i == m_CurrentClip.m_CurrentIndex)
		{
			m_NumbrdBoxes[i].Render(_renderer, m_Position, m_HighlightedBoxColour, zoomVector, false);
		}
		else
		{
			m_NumbrdBoxes[i].Render(_renderer, m_Position,zoomVector,false);
		}
	}

	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->Render(_renderer);
	}
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Render(_renderer);
	}


}

void AnimationClipEditor::LoadDefaultAssets()
{
	if (m_ResMan != nullptr)
	{
		m_ResMan->LoadDefaultMedia();
		m_CurrentTexture =m_ResMan->LoadTexture("Assets//SpriteSheets//Duran//seikendensetsu3_duran_sheet.png");

		m_EditorIconsTexture = m_ResMan->LoadTexture("Assets//editor//sprite editor icons.png");
		
		m_SpriteSheet.m_RenderInterface.texture = m_CurrentTexture;
		m_SpriteSheet.m_Size = Vector2{ (float)m_CurrentTexture->GetWidth(),(float)m_CurrentTexture->GetHeight() };
		m_SpriteSheet.m_RenderInterface.srcRect.w = m_CurrentTexture->GetWidth();
		m_SpriteSheet.m_RenderInterface.srcRect.h = m_CurrentTexture->GetHeight();
		
		m_DefaultFont = m_ResMan->LoadFont("Assets//Fonts//LucidaBrightRegular.ttf",16);
		//m_DefaultFont =m_ResMan->GetFont("Assets//Fonts//arial.ttf");

		
	}
}

void AnimationClipEditor::OpenAddFrameWindow()
{
	bool found = false;

	//If the add frame window is not within the 
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		if (m_EditorWindows[i] == m_AddFrameWindow)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		//set can delete to false;
		m_AddFrameWindow->m_ReadyForDelete = false;
		m_AddFrameWindow->MouseMove((unsigned int)m_MousePos.x, (unsigned int)m_MousePos.y);
		m_EditorWindows.push_back(m_AddFrameWindow);
	}
}

void AnimationClipEditor::OpenAnimationWindow()
{
	bool found = false;
	//If the add frame window is not within the 
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		if (m_EditorWindows[i] == m_AnimationWindow)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		//set can delete to false;
		m_AnimationWindow->m_ReadyForDelete = false;
		m_AnimationWindow->MouseMove((unsigned int)m_MousePos.x, (unsigned int)m_MousePos.y);
		m_EditorWindows.push_back(m_AnimationWindow);
	}
}

void AnimationClipEditor::LoadClip()
{
	WindowOpener opener = WindowOpener();
	std::string path = opener.PrintAndOpenStuff();
	if (path != "CANCELED")
	{
		m_CurrentClip = AnimationClip();// "clear the previous animation clip then load
		m_CurrentClip.LoadClipFromFile(path, m_ResMan);

		//m_CurrentClip.m_Looping = true;
		m_SpriteSheet.m_RenderInterface.texture = m_CurrentClip.m_SourceTexture;
		m_SpriteSheet.m_Size = Vector2{ (float)m_CurrentClip.m_SourceTexture->GetWidth(),(float)m_CurrentClip.m_SourceTexture->GetHeight() };
		m_SpriteSheet.m_RenderInterface.srcRect.w = (int)m_SpriteSheet.m_Size.x;
		m_SpriteSheet.m_RenderInterface.srcRect.h = (int)m_SpriteSheet.m_Size.y;
		//m_CurrentClip.Play();
		GenerateNumberedBoxes();
		if (m_AnimationWindow != nullptr)
		{
			m_AnimationWindow->SetClip(&m_CurrentClip);
			m_AddFrameWindow->SetClip(&m_CurrentClip);
		}
	}
}

void AnimationClipEditor::SaveClip()
{
	WindowOpener opener = WindowOpener();
	std::string path = opener.SaveFileStuff();
	if (path != "thisisnotasavelocation")
	{
		m_CurrentClip.SaveClipToFilePath(path);
	}
}

void AnimationClipEditor::ZoomIn()
{
	m_Zoom += 0.1f;
}

void AnimationClipEditor::ZoomOut()
{
	m_Zoom -= 0.1f;
}

void AnimationClipEditor::GiveTopBarBox(Box _topBarBox)
{
	m_TopBarBox = _topBarBox;

	//Resize the  height of these buttons
	m_NewFileButton.SetSize({ m_NewFileButton.GetSize().x,m_TopBarBox.h });
	m_OpenAddFrameWindow.SetSize({ m_OpenAddFrameWindow.GetSize().x,m_TopBarBox.h });
	m_OpenAnimationWindow.SetSize({ m_OpenAnimationWindow.GetSize().x,m_TopBarBox.h });
	//Bit hardcoded but our order will be ->> new clip ->AddFrame ->Animation
	m_NewFileButton.SetPosition(m_TopBarBox.pos);
	m_OpenAddFrameWindow.SetPosition(m_NewFileButton.GetPosition() + Vector2{m_NewFileButton.GetSize().x,0});
	m_OpenAnimationWindow.SetPosition(m_OpenAddFrameWindow.GetPosition() + Vector2{ m_OpenAddFrameWindow.GetSize().x,0 });
}

//Create new animation clip file, allows you to choose for a texture and then creates a 
// animation clip based on this filepath
void AnimationClipEditor::CreateNewFile()
{
	WindowOpener opener = WindowOpener();
	std::string path = opener.PrintAndOpenPng();
	if (path != "CANCELED")
	{
		//replace 


		m_CurrentClip = AnimationClip();// 
		m_CurrentClip.m_SourceTexture = m_ResMan->LoadTexture(path);

		m_SpriteSheet.m_RenderInterface.texture = m_CurrentClip.m_SourceTexture;
		m_SpriteSheet.m_Size = Vector2{ (float)m_CurrentClip.m_SourceTexture->GetWidth(),(float)m_CurrentClip.m_SourceTexture->GetHeight() };
		m_SpriteSheet.m_RenderInterface.srcRect.w = (int)m_SpriteSheet.m_Size.x;
		m_SpriteSheet.m_RenderInterface.srcRect.h = (int)m_SpriteSheet.m_Size.y;

		GenerateNumberedBoxes();
		if (m_AnimationWindow != nullptr)
		{
			m_AnimationWindow->SetClip(&m_CurrentClip);
			m_AddFrameWindow->SetClip(&m_CurrentClip);
		}
	}
}
