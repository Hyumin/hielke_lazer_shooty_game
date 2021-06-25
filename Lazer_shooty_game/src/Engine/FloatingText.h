#pragma once
#include "Text.h"
class FloatingText :
    public Text
{
public:
    FloatingText();
    FloatingText(float _dur,float _speed,std::string _text);
    ~FloatingText();

	virtual void Update(float _dt);
	virtual void Render(SDLRenderer* _renderer, Vector2 _worldpos,int _layer);
	virtual void RenderToTarget(SDLRenderer* _renderer, RenderTarget* _target, Vector2 _worldpos);
	virtual void SetText(const std::string& _string);
	virtual void SetText(const char* _text);
	virtual std::string& GetText() { return m_Text; }
	virtual void SetFont(TTF_Font* _font);
	virtual TTF_Font* GetFont() { return m_FontPointer; }
	virtual void SetColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a);
	virtual void SetColour(const SDL_Color& _color);
	virtual SDL_Color GetColour() { return m_Colour; }

	bool m_MarkedForRemoval;

private:
	float m_timer;
	float m_Duration;
	float m_Speed;

};

