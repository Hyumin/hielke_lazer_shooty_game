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


protected:

	virtual void Init(Vector2 _dir, Vector2 _vel, Vector2 _initial_pos) =0;
	Vector2 m_direction;
	Vector2 m_velocity;
	Vector2 m_position;
	float m_acceleration;
	float m_dmg;
	Box m_box;


};