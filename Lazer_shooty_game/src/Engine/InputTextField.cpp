#include "InputTextField.h"
#include <functional>
#include <iostream>

InputTextField::InputTextField()
{
	Init(InputTextMode::Text, "InputText");
}

InputTextField::InputTextField(InputTextMode _mode, const std::string& _name)
{
	Init(_mode,_name);
}

InputTextField::~InputTextField()
{

}

void InputTextField::SetTextColour(SDL_Colour _col)
{
	m_TextColour = _col;
	m_NameTextField.SetColour(m_TextColour);
	m_InputText.SetColour(m_TextColour);
}

void InputTextField::SetButtonColour(SDL_Colour _norm, SDL_Colour _hovered, SDL_Colour _clicked)
{
	m_NormalCol = _norm;
	m_HoveredCol = _hovered;
	m_ClickedCol = _clicked;
	//Makes the button render with a black normal, white clicked and dark grey hovered colours
	m_InputButton.SetWireFrameMode(m_NormalCol, m_HoveredCol,m_ClickedCol);

}

//Default initialization
void InputTextField::Init(InputTextMode _mode, const std::string& _name)
{

	m_Pos = { 100,50 };
	m_Size = {100,40};
	m_Layer = 2;

	m_Mode = _mode;
	m_InputButton = Button{};
	m_InputButton.SetLayer(m_Layer);
	m_InputButton.SetCallbackFunction(std::bind(&InputTextField::ButtonCallback, this));

	m_InputText = TextField();
	m_NameTextField = TextField();
	m_NameTextField.SetText(_name);
	//Default initialization of colours
	m_NameTextField.m_Size = { 100,20 };
	SetTextColour({0,0,0,255});
	SetButtonColour({ 0,0,0,255 }, { 75,75,75,255 }, {255,255,255,255});
	m_NameTextField.SetColour(0,0,0,255);
	m_InputText.SetColour(0, 0, 0, 255);
	m_InputText.m_Size = m_Size;
	m_NameOffset = {-25,0};

	m_Text =  _mode==InputTextMode::Numbers ?"0" : " " ;
	m_CharLimit = 30;

	m_InputText.SetText(m_Text);
	m_Focused = false;
	SetSize(m_Size);
	SetPosition(m_Pos);
	m_Changed = false;
	m_ShiftHeld = false;
}

void InputTextField::Update(float _dt)
{
	if (m_Focused)
	{
		m_FocusTimer += _dt;
		if (m_FocusTimer >= m_FocusInterval)
		{
			m_FocusTimer -= m_FocusInterval;
			m_Stripey = m_Stripey ? false : true;
		}
		if (m_Stripey)
		{
			m_InputText.SetText(m_Text + "|");
		}
		else
		{
			m_InputText.SetText(m_Text + " ");
		}
	}

}

void InputTextField::MouseDown(int _key)
{
	m_InputButton.MouseDown(_key);
}

void InputTextField::MouseUp(int _key)
{
	m_InputButton.MouseUp(_key);
	//if we're focused and we mouse up  remove focus
	if (m_Focused)
	{
		Box b = m_InputButton.GetCollider();
		if (!b.BoxCollision(b, m_MousePos))
		{
			UnFocus();
		}
	}
}

void InputTextField::MouseMove(int _x, int _y)
{
	m_InputButton.MouseMove(_x, _y);
	m_MousePos.x = _x;
	m_MousePos.y = _y;
}

void InputTextField::KeyDown(unsigned int _key)
{
	if (m_Focused)
	{
		//To enable capitals and some special characters
		if (_key == SDLK_LSHIFT || _key == SDLK_RSHIFT)
		{
			m_ShiftHeld = true;
		}
	}
}

void InputTextField::KeyUp(unsigned int _key)
{
	
	if (m_Focused)
	{
		if (_key == SDLK_LSHIFT || _key == SDLK_RSHIFT)
		{
			//We return since we don't want shift release to trigger a letter
			m_ShiftHeld = false;
			return;
		}
		//If we press backspace remove the last index of the string
		if (_key == SDLK_BACKSPACE)
		{
			if (m_Text.length() > 0)
			{
				m_Text.pop_back();
				m_InputText.SetText(m_Text.c_str());
				return;
			}
		}
		//Unfocused when we press enter
		if (_key == SDLK_RETURN)
		{
			UnFocus();
			return;
		}
		//This should make it so that anything that is not a number on
		// my normal keyboard will get discarded
		if (m_Mode == InputTextMode::Numbers)
		{
			int num = _key - (int)'0';
			//We opt for an early out if we're trying to NOT insert a number or dot in this mode
			if ((num < 0 || num > 9 )&& _key != SDLK_PERIOD&&_key != SDLK_MINUS)
			{
				return;
			}
		}
		//Cap the size like this
		if (m_Text.length() < m_CharLimit)
		{
			unsigned char charToAdd;

			//If we use underscore we should be getting an underscore
			if (_key == SDLK_MINUS&&m_ShiftHeld)
			{
				charToAdd = '_';
			}
			else
			{
				//Convert the key to a char it should be one to one with what SDL gives us
				charToAdd = (char)_key;
				if (m_ShiftHeld)
				{
					charToAdd = toupper(charToAdd);
				}
			}

			m_Text.push_back(charToAdd);
			SetText(m_Text.c_str(),true); //Settext will set the changed to true;
		}
	}
}

void InputTextField::SetFont(TTF_Font* _font)
{
	m_Font = _font;
	m_InputText.SetFont(_font);
	m_InputButton.SetFont(_font);
	m_NameTextField.SetFont(_font);
}

const TTF_Font* InputTextField::GetFont()
{
	return m_Font;
}

void InputTextField::SetPosition(Vector2 _pos)
{
	m_Pos = _pos;
	m_InputButton.SetPosition(_pos);
	m_InputText.m_pos = _pos;
	m_NameTextField.m_pos = _pos + m_NameOffset;
}

Vector2 InputTextField::GetPosition()
{
	return m_Pos;
}

void InputTextField::SetSize(Vector2 _size)
{
	m_Size = _size;
	m_InputButton.SetSize(_size);
	m_InputText.m_Size = _size;
}

Vector2 InputTextField::GetSize()
{
	return m_Size;
}

void InputTextField::Render(SDLRenderer* _renderer)
{
	m_InputButton.Render(_renderer);
	try
	{
		m_InputText.Render(_renderer, { 0,0 }, m_Layer);
		m_NameTextField.Render(_renderer, { 0,0 }, m_Layer);
	}
	catch (std::exception& e)
	{
		printf(e.what()+ '\n');
	}
}

void InputTextField::SetName(std::string& _name)
{
	m_NameTextField.SetText(_name.c_str());
}

void InputTextField::SetText(const std::string& _text, bool _change)
{
	m_Text = _text;
	m_InputText.SetText(m_Text);
	m_Changed = _change;
	//Very important when external sources are changing the text remove focus
	if (!_change)
	{
		UnFocus();
	}
}

void InputTextField::ButtonCallback()
{
	m_Focused = m_Focused ? false : true;
	if (!m_Focused)
	{
		UnFocus();
	}
}

void InputTextField::UnFocus()
{
	m_Focused = false;
	m_InputText.SetText(m_Text);
}


