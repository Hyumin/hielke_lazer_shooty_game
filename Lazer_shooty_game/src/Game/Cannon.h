#pragma once
#include "..\Engine\Object.h"
#include "..\HielkMath.h"
#include "player_stats.h"

class AnimationClip;
class TextField;
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
	void MouseMove(Vector2 _mpos);//Gives the cannon the mouse position


	Projectile* Shoot(); //returns a bullet if available if it can't shoot it will return a nullpointer

	void Set_Objects(Object* _barrel, Object* _foot_hold);
	void draw(SDLRenderer* _renderer);
	void Rotate(float _by);//This adds the float value to the rotation value

	void DebugMode(bool _new_val);
	void ToggleMouseMode();

	Cannon_Stats m_stats;
private:

	bool m_mouse_mode = false;
	bool m_debug_mode;
	//initialize function to make sure all constructors do the "same"
	void Init(Vector2& _pos);

	Vector2 m_pos;

	Object* m_foot_hold;
	Object* m_barrel;
	AnimationClip* m_shoot_anim;
	float m_rotation;
	float m_prev_rotation;
	Vector2 m_barrel_direction;
	TextField* m_text_field;

	float m_cd;
	float m_cd_timer;

};

