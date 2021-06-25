#include "TextFieldBuilder.h"



TextField TextFieldBuilder::BuildTextField(SDL_Color _col, const std::string& _text, TTF_Font* _font, Vector2 _pos, Vector2 _size)
{
	// TODO: insert return statement here
	TextField field = TextField();

	field.SetText(_text);
	field.SetColour(_col);
	field.SetFont(_font);
	field.m_pos = _pos;
	field.m_Size = _size;

	return field;

}

InputTextField* TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode _mode, const std::string& _name, TTF_Font* _font, Vector2 _pos, Vector2 _size, unsigned int _charlimit, SDL_Color _textCol, SDL_Color _norC, SDL_Color _hovC, SDL_Color _clickC)
{
	InputTextField* field = new InputTextField{ _mode ,_name};

	field->SetPosition(_pos);
	field->SetSize(_size);
	field->SetTextColour(_textCol);
	field->SetButtonColour(_norC, _hovC, _clickC);
	field->m_CharLimit = _charlimit;
	field->SetFont(_font);

	return field;
}
