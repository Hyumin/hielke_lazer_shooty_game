#pragma once
#include "../player_stats.h"
#include "../LazerProjectile.h"
#include "BasicLazerWeapon.h"

BasicLazerWeapon::BasicLazerWeapon()
{
	if (!CacheProjectile())
	{
		//We failed to cache
		printf("Failed to cache lazer projectile \n");
	}
	cd = 0.4f;

}

BasicLazerWeapon::~BasicLazerWeapon()
{
}

void BasicLazerWeapon::Update(float _dt)
{
	cd_timer += _dt;
}

//Should consider retruning an array,
Projectile* BasicLazerWeapon::Fire(Vector2 _start, Vector2 _direction)
{
	if (cd_timer >= playerStats->m_Proj_cd)
	{
		//Copy the projectile and shoot it return as pointers
		Projectile* proj = new LazerProjectile();
		proj->SetPos(_start);
		proj->SetDirection(_direction);
		proj->SetDamage(playerStats->m_Proj_dmg);
		proj->SetVelocity(_direction * playerStats->m_Proj_speed * LazerProjectile::DEFAULT_SPEED);

		cd_timer = 0;
		return proj;
	}
	else
	{
		return nullptr;
	}

}

Projectile* BasicLazerWeapon::Fire(Vector2 _start, Enemy* _target)
{
	return nullptr;
}

Projectile* BasicLazerWeapon::Fire(Vector2 _start, Cannon* _target)
{
	return nullptr;
}

void BasicLazerWeapon::SetStats(Cannon_Stats* _stats)
{
	playerStats = _stats;
}

//We cache but it doesn't work properly :(
bool BasicLazerWeapon::CacheProjectile()
{
	LazerProjectile* proj = new LazerProjectile({ 1,0 }, { 1,1 }, { 0,0 });
	base_proj = proj;
	return base_proj != nullptr;
}
