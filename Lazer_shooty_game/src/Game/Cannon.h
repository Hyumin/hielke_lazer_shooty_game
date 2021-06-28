#pragma once
#include "..\Engine\Object.h"
#include "..\HielkMath.h"


class Projectile;
class Cannon
{
	//Cannon
public:
	Cannon();
	Cannon(Vector2& _pos);
	Cannon(float _x, float _y);
	~Cannon();


	void Update(float _dt);

	void Set_Position(Vector2& _pos);
	void Set_Position(float _x, float _y);
	const Vector2& Get_Position();

	void Shoot();

	void Set_Objects(Object* _barrel, Object* _foot_hold);
	void draw(SDLRenderer* _renderer);
	void Rotate(float _by);//This adds the float value to the rotation value

	void DebugMode(bool _new_val);

private:

	bool m_debug_mode;
	//initialize function to make sure all constructors do the "same"
	void Init(Vector2& _pos);

	Vector2 m_pos;

	Object* m_foot_hold;
	Object* m_barrel;
	float m_rotation;
	float m_prev_rotation;
	Vector2 m_barrel_direction;

	float m_bull_accel;

	//Later create a projectile class for now just have 2 vectors
	std::vector<Projectile*> m_bullets;

};

