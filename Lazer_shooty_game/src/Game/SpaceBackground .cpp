#include "SpaceBackground .h"


SpaceBackground::SpaceBackground(SDL_Colour _bg_col, Vector2 _size)
{
	Init(_bg_col, _size);
}

SpaceBackground::~SpaceBackground()
{
}

void SpaceBackground::Init(SDL_Colour _bg_col, Vector2 _size)
{
	m_size = _size;
	m_background.col = _bg_col;
	m_background.box = { 0,0,(int)_size.x, (int)_size.y };
	m_pos = Vector2{ 0,0 };
	
}



void SpaceBackground::Update(float _dt)
{
	m_random_timer += _dt;

	if (m_random_timer >= m_next_time)
	{
		m_random_timer -= m_next_time;
		m_next_time = 1;
		SpawnStar(random_range(m_min_star_spawned,m_max_star_spawned));
	}

	float dead_zone = m_pos.x - 500;
	for (unsigned int i = 0; i < m_stars.size(); ++i)
	{
		SpaceBackgroundUnit& u = m_stars[i];
		u._vel.x -= u._acc * _dt;
		u._pos += u._vel *_dt;
		u._box.box.x = u._pos.x;
		u._box.box.y = u._pos.y;
		if (u._pos.x <= dead_zone)
		{
			//remove this star if its too far in the minus
			m_stars.erase(m_stars.begin() + i);
		}
	}
}

void SpaceBackground::SpawnStar(unsigned int _num)
{

	float max_accel= 100.0f;
	float max_height = 3;
	float max_width = 30;

	

	for (unsigned int i = 0; i < _num; ++i)
	{
		int rectangluar = random_range(0, 10);
		bool do_rect = rectangluar > 3;
		//Generate a start and spawn it yolooooo
		SpaceBackgroundUnit star;
		star._pos.x = m_pos.x + m_size.x + random_range(0.0f,m_random_range);
		star._pos.y = random_range(0.0f, m_size.y);
		star._acc = random_range(20.1f, max_accel);
	
		SDL_Color col = { 255,255,255,255 };
		col.r = random_range(0,255);
		col.g = random_range(0,255);
		col.b = random_range(0,255);

		FilledBox fb;
		fb.col = col;
		
		fb.box.h = rectangluar ? random_range(1.0f, max_height) : 1;
		fb.box.w = rectangluar ? fb.box.h : random_range(1.0f, max_width);
		fb.box.x = star._pos.x;
		fb.box.y = star._pos.y;

		star._box = fb;
		m_stars.push_back(star);
	}
}

void SpaceBackground::Render(SDLRenderer* _ref)
{
	_ref->DrawFilledBox(m_background, { 0,0 }, m_layer);

	for (unsigned int i = 0; i < m_stars.size(); ++i)
	{
		_ref->DrawFilledBox(m_stars[i]._box, { 0,0 }, m_layer);
	}
}

