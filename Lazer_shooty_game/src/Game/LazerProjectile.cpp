#include "LazerProjectile.h"
#include "..\Engine\ManagerSingleton.h"


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
	delete m_sprite;
	delete m_anim_clip;
}

void LazerProjectile::Update(float _dt)
{


	if (m_untargatable)
	{
		m_anim_clip->Update(_dt);
		if (m_anim_clip->m_IsFinished)
		{
			m_can_delete = true;
		}
	}
	else
	{
		m_velocity += m_direction * m_acceleration;
		m_position += m_velocity * _dt;
		m_box.pos = m_position;
		m_Circle.pos = m_position;
	}
}

void LazerProjectile::Render(SDLRenderer* _renderer, Vector2 _world_pos)
{
	m_sprite->m_Pos = m_position - (m_sprite->m_Size/2);
	m_sprite->m_RenderInterface.srcRect = m_anim_clip->GetRect();
	m_sprite->Render(_renderer, _world_pos,3);

	if (m_debug)
	{
		_renderer->DrawCircle(m_Circle, { 0,255,0,255 }, { 0,0 },1, HDEFAULTEBUGLAYER);
	}
}

void LazerProjectile::Die()
{
	m_untargatable = true;
	m_anim_clip->Play();
}

void LazerProjectile::SetAngle(float _angle)
{
	if (m_sprite != nullptr)
	{
		m_sprite->m_RenderInterface.angle = _angle * 57.32484076433121;
	}
}

void LazerProjectile::Init(Vector2 _dir, Vector2 _vel, Vector2 _initial_pos)
{
	m_position = _initial_pos;
	m_velocity = _vel;
	m_direction = _dir;
	m_acceleration = 0.0f;

	m_sprite = new Object(_initial_pos, Vector2{ 64,64 });
	m_box.w = 64;
	m_box.h = 64;
	m_Circle.radius = 32;

	m_sprite->m_RenderInterface.texture = ManagerSingleton::getInstance().res_man->LoadTexture("Assets//SpriteSheets//player//lazer_projectile_Sheet.png");
	m_sprite->m_RenderInterface.point.x = 32;
	m_sprite->m_RenderInterface.point.y = 28;
	
	m_direction.Normalize();
	//m_sprite->m_RenderInterface.angle =   m_direction.y* 57.32484076433121;//Assuming direction is normalized

	m_anim_clip = new AnimationClip();
	m_anim_clip->LoadClipFromFile("Assets//AnimationClips//lazer_projectile_hitanimation", ManagerSingleton::getInstance().res_man);

	m_can_delete = false;
	m_debug = false;

}
