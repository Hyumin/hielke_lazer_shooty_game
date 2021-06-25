#pragma once
#include "..\HielkMath.h"
#include "Button.h"
#include "SDLRenderer.h"
#include "Texture.h"

class ScrollBar
{
public :
	ScrollBar();
	ScrollBar(Texture* _iconsTexture);
	~ScrollBar();

	void Update(float _dt);
	void MouseMove(int _x, int _y);
	void MouseDown(unsigned int _key);
	void MouseUp(unsigned int _key);
	void MouseScroll(int _x, int _y);

	void Render(SDLRenderer* _render);

	float GetOffset();

	void SetPosition(Vector2 _pos);
	void SetSize(Vector2 _size);
	Vector2 GetSize() { return m_Size; }
	Vector2 GetPosition() { return m_Pos; }
	void SetMinimumAndMaximum(float _min,float _max);

	bool m_OffsetChanged;
private:
	void Init(Texture* _iconsTexture);
	void UpdateIndicator();
	void UpdateOffset();
	//When updating the difference in position/size we require the delta
	void UpdatePosition(Vector2 _pos);
	void UpdateSize(Vector2 _size);

	void ScrollButtonCallback(int _direction);

	//buttons
	Button m_ScrollNegativeBtn, m_ScrollPositiveBtn;

	std::vector<Button*> m_Buttons;//array of all buttons to not have to call every button indivdually

	SDL_Color m_NormalColour;
	SDL_Color m_HeldColour;
	Box m_ScrollBox;
	Box m_Indicator;
	Vector2 m_MousePos;

	bool m_ScrollBarHeld;


	Vector2 m_Pos;
	Vector2 m_Size;

	//We do not support diagonal scroll bars, so the offset is one dimensional,
	//Define in either an enum or something else whether or not we apply it to
	//a horizontal or vertical axis
	float m_Offset; //not public because this class needs to edit this variable
	float m_MaxOffset;
	float m_MinOffset;

	Vector2 m_ButtonSizePercentages;

};