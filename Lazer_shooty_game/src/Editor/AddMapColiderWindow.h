#include "MapEditorWindow.h"
#include "..\Engine\Button.h"
#include "..\Engine\InputTextField.h"



enum class MapColliderWindowMode
{
	ColliderDragging, ColiderSelecting,Default
};

class AddMapColliderWindow : public virtual MapEditorWindow
{
public :
	AddMapColliderWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture);
	AddMapColliderWindow();
	virtual ~AddMapColliderWindow();
	virtual void Update(float _dt);
	virtual void MouseDown(unsigned int _key);
	virtual void MouseUp(unsigned int _key);
	virtual void MouseMove(unsigned int _x, unsigned int _y);
	virtual void KeyDown(unsigned int _key);
	virtual void KeyUp(unsigned int _key);
	virtual void SetFont(TTF_Font* _font);
	virtual void SetMap(Hielke::Map* _map);

	virtual void Render(SDLRenderer* _renderer);


protected:

	virtual void Init(Texture* _IconsTexture);
	virtual void ReScaleContent();
	virtual void Reposition();
	void CreateBox();
	void SetDragMode();
	void SetSelectMode();

	void UpdateInputTextFields();//Will change the text of the input textfields according to current stats
								 // of the selected box, OR change the box if input happened.
	void RepositionButtons();
	void RepositionInputTexts();

	bool m_ColliderDragging,m_ColliderSelecting;
	Vector2 m_ColliderStart;
	Vector2 m_ColliderEnd;
	Vector2 m_SelectOffset;

	Button m_AddButton,m_SelectButton,m_DragButton;
	InputTextField* m_XText,* m_YText,* m_WText,* m_HText;

	std::vector<Button*> m_Buttons;
	std::vector<InputTextField*> m_InputTexts;

	MapColliderWindowMode m_Mode;
	Box m_CurrentSelectedBox;
};