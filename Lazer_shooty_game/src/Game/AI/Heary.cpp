#include "Heary.h"
#include "HearyAnimController.h"
#include "AnimationClip.h"
#include <ManagerSingleton.h>

Heary::Heary(Vector2 _pos, EnemyStats _stats)
{
	Init(_pos, _stats);
}

Heary::Heary(float _x, float _y, EnemyStats _stats)
{
	Init({_x,_y}, _stats);
}

Heary::~Heary()
{
}

void Heary::Update(float _dt)
{
	if (m_controller != nullptr)
	{
		m_controller->Update(_dt);
	}

	if (!m_Salto)
	{
		m_toimer += _dt;
		if (m_toimer >= 7)
		{
			m_toimer = 0;
			m_Salto = true;
		}
	}
	else
	{
		m_salto_rotation += 3 * _dt;
		if (m_salto_rotation >= 7)
		{
			m_salto_rotation = 0;
			m_Salto = false;
		}
	}

	//Movement
	CalcDirection();
	Vector2 speed_val = m_direction * m_stats.acceleration * _dt;
	m_weird_speed += m_stats.acceleration * _dt;

	if (m_weird_speed >= 500)
	{
		m_weird_speed = 500;
	}
	m_vel = m_direction * m_weird_speed;

	m_pos += m_vel * _dt;
	m_circle.pos = m_pos;
}

void Heary::Render(SDLRenderer* _renderer)
{
	m_object.m_Pos = m_pos - m_object.m_Size / 2;
	m_object.Render(_renderer, { 0,0 });

	AnimationClip* clip = m_controller->GetClip();
	m_object.m_RenderInterface.srcRect = clip->GetRect();
	if (m_debug)
	{
		_renderer->DrawCircle(m_circle, { 255,0,0,255 }, { 0,0 }, 1.0f, HDEFAULTEBUGLAYER);
		_renderer->AddLine(m_pos, m_pos + m_vel * 1, { 0,0 }, { 255,255,255,255 }, HDEFAULTEBUGLAYER);
	}
}

bool Heary::TakeDamage(float _dam)
{
	m_stats.current_health -= _dam;
	if (m_stats.current_health <= 0)
	{
		m_dead = true;
		return true;
	}
	return false;
}

void Heary::SetSize(float _x, float _y)
{
	m_object.m_Size.x = _x;
	m_object.m_Size.y = _y;

	m_circle.radius = _x / 4;
}

void Heary::UpdateHealthBars()
{
}

void Heary::InitController()
{
	m_controller = new HearyAnimController();
	if (!m_controller->LoadClips())
	{
		//Very bad :(
		printf("Could not load in heary animation clips : ( \n");
	}
	m_controller->SetOwner(this);
}

void Heary::Die()
{
	m_dead = true;
}

void Heary::Init(Vector2 _pos, EnemyStats _stats)
{
	m_stats = _stats;
	m_pos = _pos;
	m_dead = false;

	Object _obj = Object{ m_pos, {100,100} };
	m_circle.radius = (_obj.m_Size.x / 4);
	m_circle.pos = m_pos;

	_obj.m_RenderInterface.texture = ManagerSingleton::getInstance().res_man->LoadTexture("Assets//SpriteSheets//enemy//heary_sheet.png");
	m_object = _obj;

	InitController();

}

void Heary::CalcDirection()
{
	if (!m_Salto)
	{
		m_direction = { -1,0 };
	}
	else
	{
		m_direction = {-cosf(m_salto_rotation),-sinf(m_salto_rotation)};
	}
}
