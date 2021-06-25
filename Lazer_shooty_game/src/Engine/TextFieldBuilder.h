#pragma once
#include "TextField.h"
#include "InputTextField.h"

class TextFieldBuilder
{
public :
	static TextField BuildTextField(SDL_Color _col,const std::string& _text, TTF_Font* _font,Vector2 _pos, Vector2 _size);
	static InputTextField* BuildInputTextField(InputTextField::InputTextMode _mode, const std::string& _name,TTF_Font* _font, Vector2 _pos, Vector2 _size, unsigned int _charlimit,SDL_Color _textCol,SDL_Color _norC,SDL_Color _hovC, SDL_Color _clickC);

};

