#include "EnemySpawner.h"
#include "../Game.h"

//
EnemySpawner::EnemySpawner()
{
	m_game_ref = nullptr;
	m_res_manager = nullptr;
	m_mad_eye_template = nullptr;
	m_debugmode = false;
	m_spawn_timer = 0.0f;
	m_pos = Vector2{0,0};
}

//
EnemySpawner::~EnemySpawner()
{
	delete m_mad_eye_template;
	m_mad_eye_template = NULL;
}

void EnemySpawner::Update(float _dt)
{
	m_spawn_timer += _dt;

	if (m_spawn_timer >= 5.0f)
	{
		m_spawn_timer -= 5.0f;
		if (m_game_ref != nullptr)
		{
			m_game_ref->AddEnemey(SpawnMadEye());
		}
	}
}

void EnemySpawner::ToggleDebug()
{
	m_debugmode= m_debugmode ? false : true;
}

MadEye* EnemySpawner::SpawnMadEye()
{
	//This will spawn in a mad_eye if a game is available
	if (m_game_ref != NULL)
	{
		if (m_mad_eye_template == NULL)
		{
			//Initialize template
			try
			{
				CacheMadEye();
			}
			catch (std::exception e)
			{
				printf(e.what() + '/n');
			}
		}
		//Copy template
		MadEye* enem = new MadEye(0, 0, {});
		memcpy(enem , m_mad_eye_template, sizeof(MadEye));

		//TODO ADD WAVE GENERATION OR SMETHING?
		// Check if we're in a wave of enemies
	
		// if so re-use an existing path
		// Else generate a path
		//Generate a path
		EnemyPath path = GeneratePath(10, Vector2{ 1200,0 }, Vector2{ -500,0 });

		path.ProgressPath();
		enem->m_enemy_path = path;
		enem->m_follow_path = true;
		enem->SetPosition(path.m_points[0]);

		for (unsigned int i = 0; i < path.m_points.size(); ++i)
		{
			Vector2 mr_v = path.m_points[i];
		}

		return enem;
	}
	else
	{
		throw std::exception("no game has been defined can't generate enemy like this  :( ");
	}
}
//Huh actually the positions that we are interesetd in are X the Y should be randomly generated 
EnemyPath EnemySpawner::GeneratePath(int _segments, Vector2 _start_pos, Vector2 _end_pos)
{
	EnemyPath _path;
	float next_x_pos = _start_pos.x;
	float x_to_add = abs(_start_pos.x-_end_pos.x) / (float)_segments; // Add X 

	//Create a path that goes to minus position
	for (unsigned int i = 0; i < _segments+1; ++i)
	{
		//Generate a random point along the X axis
		Vector2 new_path = Vector2{ next_x_pos,0 };
		//somehow get the screenheight here and then randomize the point like that
		new_path.y = (float)random_range(0, 720 );
		_path.AddPoint(new_path);
		next_x_pos -= x_to_add;
	}

	return _path;
}

//Initialize this class by giving it its external dependencies
// and setting default values based on these dependencies
void EnemySpawner::Initialize(Game* _game, ResourceManager* _res_man)
{
	m_game_ref = _game;
	m_res_manager = _res_man;
}

void EnemySpawner::CacheMadEye()
{
	if (m_res_manager == nullptr)
	{
		throw std::exception("Could not cache a madeye due to lack of resource manager");
	}
	//Generates a generic madeye to be copied whenever the generator spawns an enemy
	EnemyStats mad_eye_stats;
	mad_eye_stats.acceleration = 50.0f;
	mad_eye_stats.current_health = 100;
	mad_eye_stats.max_health = 100;
	mad_eye_stats.damage = 5;
	m_mad_eye_template = new MadEye({ 0,0 }, mad_eye_stats);
	m_mad_eye_template->SetSize(45, 45);
}
