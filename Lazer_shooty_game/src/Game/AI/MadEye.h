#include "Enemy.h"


class MadEye : public Enemy
{
public:
	MadEye(Vector2 _pos, EnemyStats _stats);
	MadEye(float _x, float _y, EnemyStats _stats);
	~MadEye();

	virtual void Update(float _dt);
	virtual void Render(SDLRenderer* _renderer);
	virtual void TakeDamage(float _dam);

protected:

	virtual void Die();
	void Init(Vector2 _pos, EnemyStats _stats);
	void CalcDirection();


	Vector2 m_direction;

};