#pragma once
#include "Weapon.h"

class BasicLazerWeapon :
    public Weapon
{
public:
    BasicLazerWeapon();
    ~BasicLazerWeapon();
    virtual void Update(float _dt);
    virtual Projectile* Fire(Vector2 _start, Vector2 _direction);
    virtual Projectile* Fire(Vector2 _start, Enemy* _target);// Use a enemy as target 
    virtual Projectile* Fire(Vector2 _start, Cannon* _target);// Use a player as target 
    virtual void SetStats(Cannon_Stats* _stats);
private:

    bool CacheProjectile();

};

