#pragma once
#include "Button.h"

class ButtonBuilder
{
public:
	static Button BuildButton(Vector2 _pos, Vector2 _size,unsigned int _layer,std::function<void()> _func);
	static Button BuildButtonWireFrameOrFilledRect(Vector2 _pos, Vector2 _size, unsigned int _layer, std::function<void()> _func,const std::string& _name, Button::DrawMode _drawMode,
		SDL_Colour _norm, SDL_Colour _hover, SDL_Colour _clicked, SDL_Colour _textColour);
	static Button BuildButton(Vector2 _pos, Vector2 _size, unsigned int _layer, std::function<void(int)> _func, int _val);
	static Button BuildButtonWireFrameOrFilledRect(Vector2 _pos, Vector2 _size, unsigned int _layer, std::function<void(int)> _func,int _val, const std::string& _name, Button::DrawMode _drawMode,
		SDL_Colour _norm, SDL_Colour _hover, SDL_Colour _clicked, SDL_Colour _textColour);
};

