#include "MadEye.h"
#include "..\..\Engine\ManagerSingleton.h"
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <iomanip>


MadEye::MadEye(Vector2 _pos, EnemyStats _stats)
{
	Init(_pos, _stats);
}

MadEye::MadEye(float _x, float _y, EnemyStats _stats)
{

	Init(Vector2{_x,_y}, _stats);
}

MadEye::~MadEye()
{
	
}

void MadEye::Update(float _dt)
{
	if (m_dead)
	{
		return;
	}

	CalcDirection();

	
	Vector2 speed_val = m_direction * m_stats.acceleration * _dt;
	m_vel += speed_val;
	//Decrease velocity if magnitude is too high
	//printf("magnitude = %f \n", m_vel.Magnitude());
	if (m_vel.Magnitude() > 100)
	{
		printf("we're too fast \n");
		m_vel -= speed_val;
	}

	m_pos += m_vel*_dt;
	m_collider.pos = m_pos;
	if (m_follow_path)
	{
		if (Vector2::Distance(m_pos, m_target_pos) < 50)
		{
			m_enemy_path.ProgressPath();

			if (m_enemy_path.GetFinished())
			{
				Die();
			}
		}
	}

}

void MadEye::CalcDirection()
{
	if (m_follow_path)
	{
		m_target_pos = m_enemy_path.ReturnCurrentPoint();
		m_direction = m_target_pos - m_pos;
		m_direction.Normalize();
	}
	else 
	{ 
		m_direction = m_target_pos - m_pos;
		m_direction.Normalize();
	}
}


void MadEye::Render(SDLRenderer* _renderer)
{
	m_object.m_Pos = m_pos;
	
	m_object.Render(_renderer, { 0,0 });

	if (m_debug)
	{
		_renderer->DrawBox(m_collider, { 255,0,0,255 }, { 0,0 }, HDEFAULTEBUGLAYER);
		//_renderer->AddLine(m_pos, m_pos+m_direction*50, { 0,0 }, { 255,0,255,255 }, HDEFAULTEBUGLAYER);
		_renderer->AddLine(m_pos, m_pos+m_vel*1, { 0,0 }, { 255,255,255,255 }, HDEFAULTEBUGLAYER);

		if (m_follow_path)
		{
			for (unsigned int i = 1; i < m_enemy_path.m_points.size(); ++i)
			{
				_renderer->AddLine(m_enemy_path.m_points[i - 1], m_enemy_path.m_points[i], { 0,0 }, { 255,0,0,255 },HDEFAULTEBUGLAYER);
			}
			Vector2 target_path_pos = m_enemy_path.ReturnCurrentPoint();
			_renderer->DrawBox((int)target_path_pos.x-25, (int)target_path_pos.y-25,50,50, { 255,255,0,255 }, { 0,0 }, HDEFAULTEBUGLAYER);
		}

	}

}

void MadEye::TakeDamage(float _dmg)
{
	m_stats.current_health -= _dmg;
	if (m_stats.current_health <= 0)
	{
		Die();
	}
}

void MadEye::SetSize(float _x, float _y)
{
	m_object.m_Size.x = _x;
	m_object.m_Size.y = _y;
}

void MadEye::Die()
{
	//Executes stuff to delete
	std::cout << "Mad eye is dead :( \n";
	m_dead = true;
}

void MadEye::Init(Vector2 _pos, EnemyStats _stats)
{
	m_pos = _pos;
	m_vel = Vector2{ 0,0 };
	m_stats = _stats;

	int random_x = random_range(10, 256);

	Object _obj = Object{ m_pos, {(float)random_x,(float)random_x} };
	m_collider.pos = m_pos;
	m_collider.w = _obj.m_Size.x;
	m_collider.h = _obj.m_Size.y;;
	_obj.m_RenderInterface.texture = ManagerSingleton::getInstance().res_man->LoadTexture("Assets//SpriteSheets//enemy//mad_eyeball_man.png");
	m_object = _obj;
	m_direction = Vector2(0, 0);
}
