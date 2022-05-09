#pragma once
#include "..\HielkMath.h"
#include "..\Engine\SDLRenderer.h"

//This is the base class for all projectiles
class Projectile
{
public : 
	virtual void Update(float _dt) =0;
	virtual void Render(SDLRenderer* _renderer, Vector2 _world_pos = {0,0}) =0;


	Vector2 GetPos() { return m_position; }
	void SetPos(Vector2 _new_pos) { m_position = _new_pos; }
	virtual void Die()=0;
	virtual void SetDirection(Vector2 _dir) = 0;
	virtual void SetVelocity(Vector2 _vel) { m_velocity = _vel; }
	virtual void SetDamage(float _dmg)=0;
	virtual float GetDamage() { return m_dmg; }

	void ToggleDebug() 
	{
		m_debug = m_debug ? false : true;
	} 

	bool GetDelete() {
		return m_can_delete;
	}

	Box m_box;
	CircleCollider m_Circle;

	bool m_untargatable;
	bool m_debug;

protected:

	virtual void Init(Vector2 _dir, Vector2 _vel, Vector2 _initial_pos) =0;
	Vector2 m_direction;
	Vector2 m_velocity;
	Vector2 m_position;
	float m_acceleration;
	float m_dmg;
	bool m_can_delete;


};