#include "../Engine/SDLRenderer.h"


//Struct that we will be keeping track of (will convert to class if vs whines)
struct SpaceBackgroundUnit
{
	Vector2 _pos;
	Vector2 _vel;
	float _acc;
	FilledBox _box;
};

class SpaceBackground
{
public :
	SpaceBackground(SDL_Colour _bg_col,Vector2 _size);
	~SpaceBackground();

	void Init(SDL_Colour _bg_col , Vector2 _size);

	void Update(float _dt);
	void Render(SDLRenderer* _ref);
	void SetPosition(Vector2 _a)
	{
		m_pos = _a;
	}
	Vector2 GetPosition()
	{
		return m_pos;
	}


	void SpawnStar(unsigned int m_num_stars );
	unsigned int m_layer = 0;
private:
	std::vector<SpaceBackgroundUnit> m_stars;
	FilledBox m_background;
	Vector2 m_size; // size of this background
	Vector2 m_pos; //position of the background (default to zero)

	float m_random_timer =0.0f;
	float m_next_time = 0.0f;
	float m_random_range = 200.0f;
	int m_min_star_spawned = 8;
	int m_max_star_spawned = 16;
};