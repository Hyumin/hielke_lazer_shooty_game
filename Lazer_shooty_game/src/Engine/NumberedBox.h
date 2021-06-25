#pragma once
#include "TextField.h"
#include "..\HielkMath.h"
#include "SDLRenderer.h"

class NumberedBox
{

public:
	NumberedBox(int _number = 0);
	~NumberedBox();

	void Render(SDLRenderer* _renderer, Vector2 _WorldPos = { 0,0 },bool _drawFilled = false);
	void Render(SDLRenderer* _renderer, Vector2 _WorldPos = { 0,0 }, Vector2 _ScaleMult = {1,1}, bool _drawFilled = false);
	void Render(SDLRenderer* _renderer, Vector2 _WorldPos = { 0,0 }, SDL_Color _col = {255,255,255,255}, Vector2 _ScaleMult = { 1,1 }, bool _drawFilled = false);


	void SetBox(Box _box);
	Box GetBox() { return m_Box; }
	void SetPos(Vector2 _pos);
	Vector2 GetPos() { return m_Box.pos; }
	void SetColour(SDL_Color _col);
	SDL_Color GetColour() { return m_Colour; }

	void SetFont(TTF_Font* _font);

	void SetNumber(int _number);
	int GetNumber(){ return m_Number; }

private:
	
	void UpdateTextField();
	void Init();

	Box m_Box;
	SDL_Color m_Colour;
	TextField m_Text;
	int m_Number;

};

