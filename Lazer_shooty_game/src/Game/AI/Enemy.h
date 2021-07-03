#include "..\..\HielkMath.h"
#include "..\..\Engine\SDLRenderer.h"
#include "..\..\Engine\Object.h"
#include "..\Cannon.h"


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
	virtual void TakeDamage(float _dmg_val)=0;

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
		m_target = _cannon;
	}

	Box m_collider;
protected:

	virtual void Die()=0;
	Cannon* m_target = nullptr;
	bool m_dead;
	Object m_object;
	EnemyStats m_stats;
	Vector2 m_pos;
	Vector2 m_vel;

};