#pragma once
#include "EditorWindow.h"
#include "..\Engine\Map.h"

class MapEditorWindow : 
	public virtual EditorWindow
{
public:
	MapEditorWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture);
	MapEditorWindow();
	virtual ~MapEditorWindow();
	virtual void Update(float _dt);
	virtual void MouseDown(unsigned int _key);
	virtual void MouseUp(unsigned int _key);
	virtual void MouseMove(unsigned int _x, unsigned int _y);
	virtual void KeyDown(unsigned int _key) = 0;
	virtual void KeyUp(unsigned int _key) = 0;
	virtual void SetFont(TTF_Font* _font);
	virtual void SetMap(Hielke::Map* _map);

	virtual void Render(SDLRenderer* _renderer);
	void PassWorldPosAndZoom(Vector2 _pos, float _zoom);

	bool m_MouseInGUI;//
	bool m_MouseInWindow;
protected:

	virtual void Init(Texture* _IconsTexture);
	virtual void ReScaleContent();
	virtual void Reposition();

	Vector2 m_WorldPos;
	float m_Zoom;
	Hielke::Map* m_Map;
};