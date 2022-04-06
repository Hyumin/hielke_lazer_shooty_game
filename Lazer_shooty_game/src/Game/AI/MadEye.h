#include "Enemy.h"
#include "../../Engine/AnimationClip.h"

class MadEyeAnimController;
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
	void InitController();

	float GetCurrSpeed() { return m_weird_speed; }
	bool m_follow_path;
	bool m_falling = false;

	virtual void Die();
protected:

	void Init(Vector2 _pos, EnemyStats _stats);
	void CalcDirection();


	Vector2 m_direction;
	float m_weird_speed;
	MadEyeAnimController* m_controller;
	AnimationClip m_walking_anim;
	Box m_health_bar_line;
	FilledBox m_health_bar;
};