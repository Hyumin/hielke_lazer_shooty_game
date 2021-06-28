#include "LazerProjectile.h"

LazerProjectile::LazerProjectile()
{
	Init({ 0,0 }, { 0,0 }, {0,0});
}

LazerProjectile::LazerProjectile(Vector2 _dir, Vector2 _vel, Vector2 _initial_pos)
{
	Init(_dir, _vel, _initial_pos);
}

LazerProjectile::~LazerProjectile()
{
}

void LazerProjectile::Update(float _dt)
{

	m_velocity += m_direction * m_acceleration;
	m_position += m_velocity*_dt;
}

void LazerProjectile::Render(SDLRenderer* _renderer, Vector2 _world_pos)
{
	m_box.pos = m_position;
	_renderer->DrawFilledBox(m_box, { 255,0,0,125 }, _world_pos, 2);
}

void LazerProjectile::Init(Vector2 _dir, Vector2 _vel, Vector2 _initial_pos)
{
	m_position = _initial_pos;
	m_velocity = _vel;
	m_direction = _dir;
	m_acceleration = 0.1f;

	m_box.w = 10;
	m_box.h = 10;
}
