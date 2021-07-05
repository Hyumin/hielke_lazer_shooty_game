#include "MadEye.h"
#include "..\..\Engine\ManagerSingleton.h"
#include <iostream>


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
	//Set the target to a nullptr don't delete it
	m_target = nullptr;
}

void MadEye::Update(float _dt)
{
	CalcDirection();

	Vector2 vec1 = { 1,1 };
	m_vel += m_direction * m_stats.acceleration * _dt;
	m_pos += m_vel;
	m_collider.pos = m_pos;
}

void MadEye::CalcDirection()
{
	if(m_target==nullptr)
	{ 
		throw std::exception("Error MadEye.cpp has no active target set make sure to set a target if you wish to update this enemy");
	}
	Vector2 vec = m_target->Get_Position();


	m_direction = vec - m_pos; 
	m_direction.Normalize();

}


void MadEye::Render(SDLRenderer* _renderer)
{
	m_object.m_Pos = m_pos;
	
	m_object.Render(_renderer, { 0,0 });

	if (m_debug)
	{
		_renderer->DrawBox(m_collider, { 255,0,0,255 }, { 0,0 }, HDEFAULTEBUGLAYER);
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
