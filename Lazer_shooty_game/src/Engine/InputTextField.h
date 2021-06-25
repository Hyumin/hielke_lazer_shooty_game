#pragma once
#include "SDLRenderer.h"
#include "Button.h"
#include "TextField.h"

class InputTextField
{
public:
	enum InputTextMode
	{
		Text,
		Numbers
	};

	InputTextField();
	InputTextField(InputTextMode _mode, const std::string& _name);
	~InputTextField();

	void Update(float _dt);
	void MouseDown(int _key);
	void MouseUp(int _key);
	void MouseMove(int _x, int _y);
	void KeyDown(unsigned int _key);
	void KeyUp(unsigned int _key);

	void SetFont(TTF_Font* _font);
	const TTF_Font* GetFont();

	void SetPosition(Vector2 _pos);
	Vector2 GetPosition();

	void SetSize(Vector2 _size);
	Vector2 GetSize();

	void Render(SDLRenderer* _renderer);

	Vector2 m_NameOffset;
	unsigned int m_CharLimit;

	void SetName(std::string& _name);
	void SetText(const std::string& _text, bool _change = false); //the bool expression will allow you to set m_Changed to true so other classes
	// know text has been changed
	std::string GetName() {
		return m_NameTextField.GetText();
	}
	std::string GetText() {
		if (m_Text.size() == 0)
		{
			return "0";
		}
		return m_Text;
	}

	void SetTextColour(SDL_Colour _col);
	void SetButtonColour(SDL_Colour _norm, SDL_Colour _hovered, SDL_Colour _clicked);

	bool m_Changed;

private:

	void Init(InputTextMode _mode, const std::string& _name);

	bool  m_Focused;

	void ButtonCallback();
	void UnFocus();

	Vector2 m_Pos;
	Vector2 m_Size;
	Vector2 m_MousePos;

	//Colour of the text
	SDL_Colour m_TextColour;
	SDL_Colour m_HoveredCol, m_NormalCol, m_ClickedCol;

	TextField m_InputText;// The text object in which we display the text we are editing by typing
	TextField m_NameTextField;//The text object that shows what text we are editing e.g: name
	Button m_InputButton;

	TTF_Font* m_Font;

	std::string m_Text;
	InputTextMode m_Mode;

	int m_Layer;

	//To add the flashing | stripey? streepje in dutch xD little line???
	float m_FocusTimer = 0.0f;
	float m_FocusInterval = 0.3f;
	bool m_Stripey = true;
	
	bool m_ShiftHeld = false;

};

