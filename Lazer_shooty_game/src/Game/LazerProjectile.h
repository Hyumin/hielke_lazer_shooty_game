#include "projectile.h"


class LazerProjectile: public Projectile
{

public :
	LazerProjectile();
	LazerProjectile(Vector2 _dir, Vector2 _vel, Vector2 _initial_pos);
	~LazerProjectile();

	virtual void Update(float _dt) ;
	virtual void Render(SDLRenderer* _renderer, Vector2 _world_pos = { 0,0 });

protected:
	virtual void Init(Vector2 _dir, Vector2 _vel, Vector2 _initial_pos);
};