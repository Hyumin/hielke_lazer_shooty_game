#include "EditorWindow.h"
#include "..\Engine\Texture.h"
#include "..\Engine\AnimationClip.h"
#include <iostream>
#include <functional>
#include <utility>


EditorWindow::EditorWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	Init(_IconsTexture);

}

EditorWindow::EditorWindow()
{
	m_Pos = Vector2{ 0,0 };
	m_Name = "empty window";
	Init(nullptr);
}

EditorWindow::~EditorWindow()
{
	
}


void EditorWindow::Init(Texture* _IconsTexture)
{
	m_Layer = 1;
	m_Bar = Box{ 100, 100, 300, 40 };
	m_ContentBox = Box{ 100,140,300,500 };
	m_ContentScaler = Box{ 0,0,40,40 };

	m_ContentScaler.pos = m_ContentBox.pos + Vector2{ m_ContentBox.w,m_ContentBox.h };

	m_BarRelativePos = Vector2{ 0,0 };
	m_CrossRelativePos = Vector2{ 250,0 };
	m_ContentRelativePos = Vector2{ 0,40 };

	m_Dragging = false;
	m_ReadyForDelete = false;

	m_MousePos = Vector2{ 0,0 };
	m_TextField = TextField();

	m_TextField.SetText(m_Name);
	m_TextField.m_Size = Vector2{ 250,30 };
	m_TextField.SetColour(255, 255, 255, 255);

	m_IconTexture = _IconsTexture;


	m_ContentScaleObject = Object();
	m_ContentScaleObject.m_RenderInterface.srcRect = { 16,16,16,16 };

	if (m_IconTexture != nullptr)
	{
		m_ContentScaleObject.m_RenderInterface.texture = _IconsTexture;
	}
	m_ExitButton = Button{};
	if (m_IconTexture != nullptr)
	{
		RenderInterface norm, clicked, hovered;
		norm.texture = _IconsTexture;
		norm.srcRect = { 48,16,16,16 };
		norm.renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;
		clicked = norm;
		hovered = norm;
		clicked.srcRect.x = 80;
		hovered.srcRect.x = 64;
		m_ExitButton.SetTextureDrawMode(norm, clicked, hovered);
	}
	m_ExitButton.SetCallbackFunction(std::bind(&EditorWindow::ExitPressed,this));

	m_ExitButton.SetSize({ 48,m_Bar.h });
	m_ExitButton.SetLayer(m_Layer);

	m_Color = { 0,160,15,255 };
	m_LightColor = {0,235,15,255};
	m_DarkColor = { 0,140,15,255 };

	m_MaxWidth = 5000;
	m_MaxHeight = 5000;
	m_MinWidth = 100;
	m_MinHeight = 100;
}


void EditorWindow::Update(float _dt)
{

	if (m_Dragging)
	{
		Reposition();
	}
	m_TextField.Update(_dt);
	m_ExitButton.Update(_dt);
}

void EditorWindow::MouseDown(unsigned int _key)
{
	if (_key ==(SDL_BUTTON_LEFT))
	{
		if (m_Bar.BoxCollision(m_Bar, m_MousePos))
		{
			//Can drag the mouse
			m_Dragging = true;
			m_StartDragPos = m_Bar.pos -m_MousePos;

		}
		if (m_ContentScaler.BoxCollision(m_ContentScaler, m_MousePos))
		{
			m_ScalingSize = true;
			m_ScaleStart = m_MousePos;
			m_ScaleOnStart = Vector2{m_ContentBox.w, m_ContentBox.h};
		}
	}
	m_ExitButton.MouseDown(_key);
}

void EditorWindow::MouseUp( unsigned int _key)
{
	// if mouse button up draggin should be false
	if (_key == (SDL_BUTTON_LEFT))
	{
		m_Dragging = false;
		m_ScalingSize = false;
	}
	m_ExitButton.MouseUp(_key);
}

void EditorWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	m_MousePos.x = (float)_x;
	m_MousePos.y = (float)_y;
	//IF we're dragging the box set box pos to be the same as mousepos
	if (m_Dragging)
	{
		float x =m_StartDragPos.x;
		float y =m_StartDragPos.y;

		m_Pos.x = _x+x;
		m_Pos.y = _y+y;
	}

	if (m_ScalingSize)
	{
		ReScaleContent();
	}
	m_ExitButton.MouseMove(_x, _y);
}

void EditorWindow::ReScaleContent()
{
	float newWidth = m_ScaleOnStart.x - (m_ScaleStart.x - m_MousePos.x);
	float newHeight = m_ScaleOnStart.y - (m_ScaleStart.y - m_MousePos.y);

	if (newWidth < m_MinWidth)
	{
		newWidth = m_MinWidth;
	}
	if (newHeight < m_MinHeight)
	{
		newHeight = m_MinHeight;
	}
	if (newWidth > m_MaxWidth)
	{
		newWidth = m_MaxWidth;
	}
	if (newHeight > m_MaxHeight)
	{
		newHeight = m_MaxHeight;
	}

	m_ContentBox.w = newWidth;
	m_ContentBox.h = newHeight;

	m_Bar.w = m_ContentBox.w;
	m_CrossRelativePos.x = m_Bar.w - m_ExitButton.GetSize().x;
	Reposition();
}

void EditorWindow::Reposition()
{
	m_Bar.pos = m_Pos + m_BarRelativePos;
	m_TextField.m_pos = m_Pos + m_BarRelativePos;
	m_ContentBox.pos = m_Pos + m_ContentRelativePos;
	m_ExitButton.SetPosition(m_Pos + m_CrossRelativePos);

	m_ContentScaler.pos = m_ContentBox.pos + Vector2{ m_ContentBox.w - m_ContentScaler.w,m_ContentBox.h - m_ContentScaler.h };

	m_ContentScaleObject.m_Pos = m_ContentScaler.pos;
	m_ContentScaleObject.m_Size = Vector2{ m_ContentScaler.w,m_ContentScaler.h };
}

void EditorWindow::SetFont(TTF_Font* _font)
{
	m_TextField.SetFont(_font);
}

void EditorWindow::SetName(std::string& _name)
{
	m_Name = _name;
	m_TextField.SetText(m_Name);
}


void EditorWindow::Render(SDLRenderer* _renderer)
{
	_renderer->DrawFilledBox(m_Bar, m_DarkColor, { 0,0 },m_Layer);
	_renderer->DrawBox(m_ContentBox, { 255,255,255,255 }, { 0,0 },m_Layer);
	m_ContentScaleObject.Render(_renderer, Vector2{ 0, 0 },m_Layer);
	m_TextField.Render(_renderer, Vector2{ 0,0 },m_Layer);

	m_ExitButton.Render(_renderer);

}

void EditorWindow::ExitPressed()
{
	m_ReadyForDelete = true;
}
