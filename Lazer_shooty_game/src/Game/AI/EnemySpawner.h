#include <HielkMath.h>
#include "MadEye.h"
class Game;
class ResourceManager;

class EnemySpawner
{
public:
	EnemySpawner();
	~EnemySpawner();

	void Initialize(Game* _game,ResourceManager* _resman);
	void Update(float _dt);
	void ToggleDebug();


	MadEye* SpawnMadEye();
	bool GetDebugMode() { return m_debugmode; }//Not sure yet what kind of debug info this should display, maybe some kind of text
	

	void CacheMadEye(); // generates a generic madeye that we will copy from instead of having to initialize one from the go again.
						//only a clone of this enemy should ever be spawned

private:
	EnemyPath GeneratePath(int _segments, Vector2 _start_position, Vector2 _end_position);
	float m_spawn_timer;
	bool m_debugmode;
	Game* m_game_ref;

	MadEye* m_mad_eye_template;
	ResourceManager* m_res_manager;
	Vector2 m_pos;

};