#pragma once
#include "../projectile.h"
//Base class for all weapon

class Cannon_Stats;
class Enemy;
class Cannon;
class Weapon
{
public:
	Weapon() {};
	~Weapon();

	virtual void Update(float _dt) =0;
	virtual Projectile* Fire(Vector2 _start, Vector2 _direction) =0;
	virtual Projectile* Fire(Vector2 _start, Enemy* _target) =0;// Use a enemy as target 
	virtual Projectile* Fire(Vector2 _start, Cannon* _target) =0;// Use a player as target 
	virtual void SetStats(Cannon_Stats* _stats) =0;

protected:

	float cd = 0.0f;// the interval between shots
	float cd_timer =0.0f;
	float dmg = 0.0f; //The dmg of each projectile
	int num_projectiles = 1; //The amount of projectile launched

	Projectile* base_proj = nullptr;
	Cannon_Stats* playerStats = nullptr;

};

