#include "MadEye.h"
#include "..\..\Engine\ManagerSingleton.h"
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <math.h>
#include "../../Engine/AnimationClip.h"
#include "MadEyeAnimController.h"



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
	delete m_controller;
}

void MadEye::Update(float _dt)
{
	if (m_controller != nullptr)
	{
		m_controller->Update(_dt);
	}
	if (m_dead|| m_falling)
	{
		m_pos.x += m_vel.x * _dt;
		return;
	}
	CalcDirection();
	Vector2 speed_val = m_direction * m_stats.acceleration * _dt;
	m_weird_speed += m_stats.acceleration * _dt;

	if (m_weird_speed >= 350)
	{
		m_weird_speed = 350;
	}
	m_vel = m_direction * m_weird_speed;

	m_pos += m_vel*_dt;
	m_circle.pos = m_pos;
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

/*

function procces (Array[] input)
{
	
	unsigned int count= 0;
	Array[] sorted_array = autosort(input);

	for (unsigned int i =1 ; i <sorted_array.length(); ++i){
	 

	}

	
}

*/
void MadEye::Render(SDLRenderer* _renderer)
{
	m_object.m_Pos = m_pos - m_object.m_Size/2;
	UpdateHealthBars();

	_renderer->DrawBox(m_health_bar_line, m_health_bar.col, { 0,0 });
	m_object.Render(_renderer, { 0,0 });
	_renderer->DrawFilledBox(m_health_bar, {0,0},1);

	AnimationClip* clip = m_controller->GetClip();
	m_object.m_RenderInterface.srcRect = clip->GetRect();
	if (m_debug)
	{
		_renderer->DrawCircle(m_circle, { 255,0,0,255 }, { 0,0 },1.0f, HDEFAULTEBUGLAYER);
		//_renderer->DrawBoxZoomed(m_collider, { 255,0,0,255 }, { 0,0 },1.0f, HDEFAULTEBUGLAYER);
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
//
bool MadEye::TakeDamage(float _dmg)
{
	m_stats.current_health -= _dmg;
	if (m_stats.current_health <= 0)
	{
		m_controller->SetState(MadEyeAnimController::AnimState::Falling);
		m_falling = true;
		m_circle.pos.x = -90000000;
		return true;
	}
	return false;
}

void MadEye::SetSize(float _x, float _y)
{
	m_object.m_Size.x = _x;
	m_object.m_Size.y = _y;

	m_circle.radius = _x/4;
}

void MadEye::UpdateHealthBars()
{
	//first define the sie of the healthbar box
	m_health_bar.box.w = m_object.m_Size.x;
	m_health_bar.box.h = m_object.m_Size.y / 10;
	//Then set position accordingly
	m_health_bar.box.x = m_object.m_Pos.x;
	m_health_bar.box.y = m_object.m_Pos.y - m_health_bar.box.h; //Subtract the height of the object so the healthbar floats above

	m_health_bar_line.pos = { (float)m_health_bar.box.x,(float)m_health_bar.box.y};
	m_health_bar_line.w = m_health_bar.box.w;
	m_health_bar_line.h = m_health_bar.box.h;
	m_health_bar.box.w *= HielkMath::clamp( m_stats.current_health / m_stats.max_health,0,m_health_bar.box.w);
}

void MadEye::InitController()
{
	if (m_controller != nullptr)
	{
		//delete m_controller;
		m_controller = nullptr;
	}
	m_controller = new MadEyeAnimController();
	if (!m_controller->LoadClips())
	{
		//printf("Animations could not be loaded \n");
	}
	else
	{
		//printf("Animations could be loaded \n");
		m_controller->m_owner = this;
	}

	m_controller->SetState(MadEyeAnimController::AnimState::Moving);
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
	m_circle.radius = (_obj.m_Size.x/4);
	m_circle.pos = m_pos;
	_obj.m_RenderInterface.texture = ManagerSingleton::getInstance().res_man->LoadTexture("Assets//SpriteSheets//enemy//mad_Eye_sheet.png");
	m_object = _obj;
	m_direction = Vector2(0, 0);

	InitController();

	m_health_bar.col = { 255,0,0,255 };
}
