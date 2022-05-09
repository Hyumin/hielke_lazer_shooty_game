#pragma once
#include "Enemy.h"

class HearyAnimController;
class Heary : public Enemy
{
public:
	Heary(Vector2 _pos, EnemyStats _stats);
	Heary(float _x, float _y, EnemyStats _stats);
	~Heary();

	virtual void Update(float _dt);
	virtual void Render(SDLRenderer* _renderer);
	virtual bool TakeDamage(float _dam);
	void SetSize(float  _x, float _y);
	void UpdateHealthBars();
	void InitController();

	float GetCurrSpeed() { return m_weird_speed; }
	bool m_follow_path;
	bool m_falling = false;

	virtual void Die();
protected:

	void Init(Vector2 _pos, EnemyStats _stats);
	void CalcDirection();


	Vector2 m_direction;
	float m_weird_speed;
	HearyAnimController* m_controller;
	Box m_health_bar_line;
	FilledBox m_health_bar;

	bool m_Salto = false;
	float m_salto_rotation = 0.0f;
	float m_toimer = 0.0f;

};

