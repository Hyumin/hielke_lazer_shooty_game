#pragma once
#include "Text.h"
class TextField :
    public virtual Text
{
public:
	TextField();
	~TextField();

	virtual void Update(float _dt);
	virtual void Render(SDLRenderer* _renderer, Vector2 _worldpos,int _layer);
	virtual void RenderToTarget(SDLRenderer* _renderer,RenderTarget* _target, Vector2 _worldpos);
	virtual void SetText(const std::string& _string);
	virtual void SetText(const char* _text);
	virtual std::string& GetText() { return m_Text; }
	virtual void SetFont(TTF_Font* _font);
	virtual TTF_Font* GetFont() { return m_FontPointer; }
	virtual void SetColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a);
	virtual void SetColour(const SDL_Color& _color);
	virtual SDL_Color GetColour() { return m_Colour; }
	TextRenderInterface GetInterface() { return m_Interface; }

private:


};

