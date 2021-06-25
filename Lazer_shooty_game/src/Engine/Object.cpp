#include "Object.h"

Object::Object()
{
	m_Pos = Vector2(0, 0);
	m_Size = Vector2{64,64};
	m_RenderInterface.srcRect = { 0,0,64,64 };
	m_RenderInterface.texture = NULL;
	m_RenderInterface.renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;
}

Object::Object(Vector2 _pos, Vector2 _size)
{
	m_Pos = _pos;
	m_Size = _size;
	m_RenderInterface.srcRect = {0,0,64,64};
	m_RenderInterface.texture = NULL;
	m_RenderInterface.renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;
}


Object::Object(Vector2 _pos, Vector2 _size, RenderInterface _visual)
{
	m_Pos = _pos;
	m_Size = _size;
	m_RenderInterface = _visual;
}

Object::Object(Vector2 _pos, Vector2 _size, SDL_Rect _srcRect, SDL_RendererFlip _flip,Texture* _tex)
{
	m_Pos = _pos;
	m_Size = _size;
	m_RenderInterface.srcRect = _srcRect;
	m_RenderInterface.texture = _tex;
	m_RenderInterface.renderFlip = _flip;
}

Object::~Object()
{
}

void Object::Update(float _dt)
{
}

void Object::Render(SDLRenderer* _Renderer, Vector2 _WorldPos = Vector2{ 0,0 },unsigned int _layer)
{
	//Update dest rect to latest
	m_RenderInterface.destRect = { (int)m_Pos.x- (int)_WorldPos.x,(int)m_Pos.y-(int)_WorldPos.y,(int)m_Size.x,(int)m_Size.y};
	
	_Renderer->AddToRenderqueue(m_RenderInterface,_layer);


}

void Object::Render(SDLRenderer* _Renderer, Vector2 _WorldPos, Vector2 _ScaleMult, unsigned int _layer )
{
	m_RenderInterface.destRect = { (int)((m_Pos.x - _WorldPos.x)*_ScaleMult.x),(int)((m_Pos.y - _WorldPos.y)*_ScaleMult.y),(int)(m_Size.x*_ScaleMult.x),(int)(m_Size.y*_ScaleMult.y) };

	_Renderer->AddToRenderqueue(m_RenderInterface,_layer);

}
