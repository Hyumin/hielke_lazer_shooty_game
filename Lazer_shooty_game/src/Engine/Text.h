#pragma once
#include "SDLRenderer.h"
#include "..\HielkMath.h"

class Text
{

public:
	Text();
	~Text();
	virtual void Update(float _dt) =0;
	virtual void Render(SDLRenderer* _renderer,Vector2 _worldpos,int _layer) = 0;
	virtual void RenderToTarget(SDLRenderer* _renderer,RenderTarget* _target,Vector2 _worldpos) = 0;
	virtual void SetText(const std::string& _string) = 0;
	virtual void SetText(const char* _text) = 0;
	virtual std::string& GetText()=0;
	virtual void SetFont(TTF_Font* _font) = 0;
	virtual TTF_Font* GetFont() =0;
	virtual void SetColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a) = 0;
	virtual void SetColour(const SDL_Color& _color) = 0;
	virtual SDL_Color GetColour() =0;


	Vector2 m_pos;
	Vector2 m_Size;


protected:
	void UpdateInterface(SDLRenderer* _renderer);
	bool  WrapText(SDL_Surface* _Mainsurface, std::string _remainingText,float _sizePerCharacter,int _ypos);

	Vector2 m_SrcSize;

	TTF_Font* m_FontPointer;
	TextRenderInterface m_Interface;

	std::string m_Text;
	SDL_Color m_Colour;
	SDL_Texture* m_Texture;
};

