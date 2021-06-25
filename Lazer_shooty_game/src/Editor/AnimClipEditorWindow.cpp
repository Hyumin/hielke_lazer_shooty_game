#include "AnimClipEditorWindow.h"
#include "..\Engine\AnimationClip.h"
#include "..\Engine\Texture.h"

AnimClipEditorWindow::AnimClipEditorWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	Init(_IconsTexture);

}

AnimClipEditorWindow::AnimClipEditorWindow()
{
	m_Pos = Vector2{ 0,0 };
	m_Name = "empty window";
	Init(nullptr);
}

AnimClipEditorWindow::~AnimClipEditorWindow()
{
	m_CurrentClip = nullptr;
}

void AnimClipEditorWindow::SetClip(AnimationClip* _clip)
{
	m_CurrentClip = _clip;
}

void AnimClipEditorWindow::Update(float _dt)
{
	EditorWindow::Update(_dt);
}

void AnimClipEditorWindow::MouseDown(unsigned int _key)
{
	EditorWindow::MouseDown(_key);
}

void AnimClipEditorWindow::MouseUp(unsigned int _key)
{
	EditorWindow::MouseUp(_key);
}

void AnimClipEditorWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	EditorWindow::MouseMove(_x,_y);
}

void AnimClipEditorWindow::SetFont(TTF_Font* _font)
{
	EditorWindow::SetFont(_font);
}

void AnimClipEditorWindow::Render(SDLRenderer* _renderer)
{
	EditorWindow::Render(_renderer);
}

void AnimClipEditorWindow::Init(Texture* _IconsTexture)
{
	EditorWindow::Init(_IconsTexture);
	m_CurrentClip = nullptr;
	m_ChangeToAnimationClip = false;

}

void AnimClipEditorWindow::ReScaleContent()
{
	EditorWindow::ReScaleContent();
}

void AnimClipEditorWindow::Reposition()
{
	EditorWindow::Reposition();
}
