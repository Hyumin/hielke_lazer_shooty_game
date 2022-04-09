#include "projectile.h"
#include "..\Engine\AnimationClip.h"
#include "..\Engine\Object.h"

class LazerProjectile: public Projectile
{

public :
	LazerProjectile();
	LazerProjectile(Vector2 _dir, Vector2 _vel, Vector2 _initial_pos);
	~LazerProjectile();

	virtual void Update(float _dt) ;
	virtual void Render(SDLRenderer* _renderer, Vector2 _world_pos = { 0,0 });
	virtual void Die();
	void Scale(float _s);//Scales the bullet and its collider based on the value of _s 1 will do nothign
	void SetAngle(float _angle);

protected:
	virtual void Init(Vector2 _dir, Vector2 _vel, Vector2 _initial_pos);
	Object *m_sprite;
	AnimationClip *m_anim_clip;

};