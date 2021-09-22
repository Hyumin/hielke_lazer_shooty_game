#include "Enemy.h"
#include "../../Engine/AnimationClip.h"

class MadEye : public Enemy
{
public:
	MadEye(Vector2 _pos, EnemyStats _stats);
	MadEye(float _x, float _y, EnemyStats _stats);
	~MadEye();

	virtual void Update(float _dt);
	virtual void Render(SDLRenderer* _renderer);
	virtual void TakeDamage(float _dam);
	void SetSize(float  _x, float _y);
	void UpdateHealthBars();

	bool m_follow_path;
protected:

	virtual void Die();
	void Init(Vector2 _pos, EnemyStats _stats);
	void CalcDirection();


	Vector2 m_direction;
	float m_weird_speed;
	AnimationClip m_walking_anim;
	Box m_health_bar_line;
	FilledBox m_health_bar;
};