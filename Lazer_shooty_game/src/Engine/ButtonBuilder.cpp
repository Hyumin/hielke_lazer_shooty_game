#include "ButtonBuilder.h"

Button ButtonBuilder::BuildButton(Vector2 _pos, Vector2 _size, unsigned int _layer, std::function<void()> _func)
{
    Button button = Button{};
    button.SetPosition(_pos);
    button.SetLayer(_layer);
    button.SetSize(_size);
    button.SetCallbackFunction(_func);

    return button;
}

Button ButtonBuilder::BuildButtonWireFrameOrFilledRect(Vector2 _pos, Vector2 _size, unsigned int _layer, std::function<void()> _func, const std::string& _name, Button::DrawMode _drawMode, SDL_Colour _norm, SDL_Colour _hover, SDL_Colour _clicked, SDL_Colour _textColour)
{
    Button button = Button{};
    button.SetPosition(_pos);
    button.SetLayer(_layer);
    button.SetSize(_size);
    button.SetCallbackFunction(_func);
    button.SetWireFrameMode(_norm, _hover, _clicked);
    button.SetText(_name);
    button.SetTextColor(_textColour);

    if(_drawMode==Button::DrawMode::FILLEDRECT)
    {
        button.SetFilledRectMode();
    }
    

    return button;
}
//build button with a void function pointer that has a int as input
//can be used for accesing a specific element in an ARRAy or something like that
Button ButtonBuilder::BuildButton(Vector2 _pos, Vector2 _size, unsigned int _layer, std::function<void(int)> _func, int _val)
{
    Button button = Button{};
    button.SetPosition(_pos);
    button.SetLayer(_layer);
    button.SetSize(_size);
    button.SetCallbackFunction(_func,_val);

    return button;
}

Button ButtonBuilder::BuildButtonWireFrameOrFilledRect(Vector2 _pos, Vector2 _size, unsigned int _layer, std::function<void(int)> _func, int _val, const std::string& _name, Button::DrawMode _drawMode, SDL_Colour _norm, SDL_Colour _hover, SDL_Colour _clicked, SDL_Colour _textColour)
{
    Button button = Button{};
    button.SetPosition(_pos);
    button.SetLayer(_layer);
    button.SetSize(_size);
    button.SetCallbackFunction(_func,_val);
    button.SetWireFrameMode(_norm, _hover, _clicked);
    button.SetText(_name);
    button.SetTextColor(_textColour);

    if (_drawMode == Button::DrawMode::FILLEDRECT)
    {
        button.SetFilledRectMode();
    }


    return button;
}
