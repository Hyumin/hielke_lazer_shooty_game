#pragma once
#include "..\..\HielkMath.h"
#include "..\..\Engine\SDLRenderer.h"
#include "..\..\Engine\Object.h"
#include "..\Cannon.h"
#include "EnemyPath.h"

struct EnemyStats
{
	float max_health;
	float current_health;
	float acceleration;
	float damage;
};

//Base class for the enemy
class Enemy
{
public:

	virtual void Update(float _dt) = 0;
	virtual void Render(SDLRenderer* _renderer) =0;
	virtual bool TakeDamage(float _dmg_val)=0;

	void SetPosition(Vector2 _pos) { m_pos = _pos; }
	void SetPosition(float _x, float _y) 
	{
		m_pos.x = _x;
		m_pos.y = _y;
	}
	Vector2 GetPosition() { return m_pos; }
	bool GetDeathState() { return m_dead; }
	bool m_debug = false;

	void SetTarget(Cannon* _cannon)
	{
		m_target_pos = _cannon->Get_Position();
	}

	Box m_collider;
	CircleCollider m_circle;
	EnemyPath m_enemy_path;

protected:

	virtual void Die()=0;
	bool m_dead;
	Vector2 m_target_pos;
	Object m_object;
	EnemyStats m_stats;
	Vector2 m_pos;
	Vector2 m_vel;

};