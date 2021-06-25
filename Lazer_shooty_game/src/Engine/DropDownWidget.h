#pragma once
#include "..\HielkMath.h"
#include "SDLRenderer.h"
#include "RenderTarget.h"
#include "Button.h"
#include "TextField.h"

class DropDownWidget
{
public :
	DropDownWidget();
	DropDownWidget(Vector2 _pos, Vector2 _size, Texture* _icons);
	~DropDownWidget();

	void SetStates(const std::vector<std::string>& _strings, const std::vector<int>& _values);
	void Update(float _dt);
	void MouseDown(int _key);
	void MouseUp(int _key);
	void MouseMove(int _x,int _y);
	void MouseScroll(int _x, int _y);
	void SetFont(TTF_Font* _font);

	void Render(SDLRenderer* _renderer);

	int GetState() { return m_CurrentState; }

	void SetPosition(const Vector2& _pos);
	void SetSize(const Vector2& _size);
	Vector2 GetSize() { return m_Size; }
	Vector2 GetPosition() { return m_Pos; }

private:

	Vector2 m_Pos;
	Vector2 m_Size;
	void Init(Texture* _icons);
	void DropDownCallback();//this is the callback for wehn we press the downwards pointing arrow
	void DropDownTextCallback(int _num);//Calback for the buttons array

	SDL_Color m_Norm, m_Hovered, m_Clicked,m_TextCol;//Colours

	bool m_Opened;
	int m_CurrentState = 0;
	int m_Layer = 0;
	Box m_ContentBox;
	Button* m_DropDownButton;
	Button* m_CurrentIdentifierButton;//This is the current enumeration butotn, it does the same as the dropdown button
									  //but it will be set to display the name of the current selected enum
	//We won't story these two buttons into the button array as they need to be always displayed, if I decide the widget 
	//needs a third button I will put it in an array

	Texture* m_IconsTexture;
	
	std::vector<Button*> m_Buttons; // The "hidden" buttons
	std::vector<int> m_StateBindings;//Which int corresponds with which button in the array

	RenderTarget* m_Target;
	TTF_Font* m_FontRef;

};