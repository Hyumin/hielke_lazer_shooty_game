#pragma once
#include "..\Engine\ResourceManager.h"
#include "..\Engine\AnimationClip.h"
#include "..\Engine\SDLRenderer.h"
#include "..\Engine\Object.h"
#include "..\HielkMath.h"
#include "..\Engine\WindowOpener.h"
#include "..\Engine\NumberedBox.h"
#include "..\Engine\Button.h"

//This class will have the following purpose:
//Creation of new animation clips for any given sprite sheet
//Editing, saving and loading of any existing clip.

class EditorWindow;
class AnimClipEditorWindow;
class AnimationWindow;
class AddFrameWindow;
class TextField;

class AnimationClipEditor
{
public:
	AnimationClipEditor();
	AnimationClipEditor(ResourceManager* _resMan);
	~AnimationClipEditor();

	void KeyDown(unsigned int _key);
	void KeyUp(unsigned int _key);
	void MouseDown(unsigned int _key);
	void MouseUp(unsigned int _key);
	void MouseMove(int _x, int _y);
	void MouseWheel(int _x, int _y);

	void Update(float _dt);
	void SetResourceManager(ResourceManager* _resMan);
	void Render(SDLRenderer* _renderer);

	void LoadClip();
	void SaveClip();
	void ZoomIn();
	void ZoomOut();
	void GiveTopBarBox(Box _topBarBox);

private:
	
	void GenerateNumberedBoxes();//fills the array m_NumbrdBoxes with boxes based on the current animationclip
	void Init();
	void LoadDefaultAssets();
	void OpenAddFrameWindow();
	void OpenAnimationWindow();

	void CreateNewFile();

	bool m_Sprinting,m_Up, m_Left, m_Down, m_Right=false;

	std::string m_ClipName;
	std::string m_FileName;

	Object m_SpriteSheet;//The image of the sprite sheet we wish to edit
	AnimationClip m_CurrentClip;
	Object m_CurrentAnimationObject;

	Vector2 m_MousePos;
	Vector2 m_Position;// "World position"

	ResourceManager* m_ResMan;//Reference to the resource manager do not delete

	Button m_NewFileButton;

	Box m_TopBarBox;

	Texture* m_EditorIconsTexture; // the texture we use for all of our editor icons
	Texture* m_CurrentTexture;
	AnimationWindow* m_AnimationWindow;
	AddFrameWindow* m_AddFrameWindow;
	TTF_Font* m_DefaultFont;

	Button m_OpenAddFrameWindow;
	Button m_OpenAnimationWindow;

	//An array that is used  to call, update , render mouse move etc. for all editor windows
	std::vector<AnimClipEditorWindow*> m_EditorWindows;
	std::vector<Button*> m_Buttons;
	std::vector<NumberedBox> m_NumbrdBoxes;

	SDL_Color m_HighlightedBoxColour;

	float m_Zoom = 1.0f;
	float m_Speed = 50.0f;
	float m_SprintMultiplier = 2.5f;

};

