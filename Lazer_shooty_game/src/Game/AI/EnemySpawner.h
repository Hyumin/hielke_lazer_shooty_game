#include <HielkMath.h>
#include "Enemy.h"


class Game;
class ResourceManager;
class Heary;
class MadEye;

class EnemySpawner
{
public:
	EnemySpawner();
	~EnemySpawner();

	void Initialize(Game* _game,ResourceManager* _resman);
	void Update(float _dt);
	void ToggleDebug();


	MadEye* SpawnMadEye();
	Heary* SpawnHeary();

	bool GetDebugMode() { return m_debugmode; }//Not sure yet what kind of debug info this should display, maybe some kind of text
	

	void CacheMadEye(); // generates a generic madeye that we will copy from instead of having to initialize one from the go again.
						//only a clone of this enemy should ever be spawned

	void CacheHeary();

private:
	EnemyPath GeneratePath(int _segments, Vector2 _start_position, Vector2 _end_position);
	float m_spawn_timer;
	bool m_debugmode;
	Game* m_game_ref;

	MadEye* m_mad_eye_template;
	Heary* m_heary_template;
	ResourceManager* m_res_manager;
	Vector2 m_pos;

};