#include "..\Engine\SDLRenderer.h"
#include "..\Engine\ResourceManager.h"
#include "..\Engine\Button.h"
#include "..\HielkMath.h"
#include "AnimtionClipEditor.h"
#include "MapEditor.h"


//The editor class will make high level decision regard to the individual editors
//It will basically decide what mode we're gonna run in, and we need to think about 
//Whether we're gonna keep references to the resource manager etc. inside of the editor
//And we'll refactor out the topbar from the animclip editor the bar, the zoom buttons, and the 
//save buttons will each still be contained in the editor the rest the anim clip editor 
// has to figure out

 enum class EditorState
{
	 AnimationClip, Map
};

class Editor
{
public:
	Editor(ResourceManager* _resman);
	~Editor();

	void KeyDown(unsigned int _key);
	void KeyUp(unsigned int _key);
	void MouseDown(unsigned int _key);
	void MouseUp(unsigned int _key);
	void MouseMove( int _x,  int _y);
	void MouseWheel(int _x, int _y);

	void Update(float _dt);
	void Render(SDLRenderer* _renderer);
	void OnWindowResize(unsigned int _width, unsigned int _height);//This function should be called whenever the SDL window gets resized and right after initialization

private:

	void Init(ResourceManager* _resman);
	void LoadAssets();

	//Callback functions
	void SaveCallback();
	void LoadCallback();
	void ZoomInCallback();
	void ZoomOutCallback();
	void SwitchModeCallBack();

	Box m_TopBar;//the giant barr where we want to place our buttons!

	MapEditor* m_MapEditor;
	AnimationClipEditor* m_AnimClipEditor;
	ResourceManager* m_ResourceManager;
	EditorState m_EditorState;

	Texture* m_EditorIconsTexture;
	TTF_Font* m_Font;

	Button m_LoadButton, m_SaveButton, m_ZoomInButton, m_ZoomOutButton, m_SwitchModeButton;
	std::vector<Button*> m_Buttons;

	SDL_Colour m_BarColour;

	unsigned int  m_WindowWidth;
	unsigned int  m_WindowHeight;


};

