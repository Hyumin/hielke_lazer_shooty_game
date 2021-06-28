#include "Cannon.h"
#include "..\Engine\ManagerSingleton.h"
#include "LazerProjectile.h"

#include <iostream>

Cannon::Cannon()
{
	Vector2 zero_vec = Vector2{ 0,0 };
	Init(zero_vec);
}

Cannon::Cannon(Vector2& _pos)
{
	Init(_pos);
}

Cannon::Cannon(float _x, float _y)
{
	Vector2 vec = { _x,_y };
	Init(vec);
}

Cannon::~Cannon()
{
	delete m_barrel;
	m_barrel = NULL;
	delete m_foot_hold;
	m_foot_hold = NULL;

	m_bullets.clear();
}

void Cannon::Update(float _dt)
{
	for (uint32_t i = 0; i < m_bullets.size(); ++i)
	{
		m_bullets[i]->Update(_dt);
		Vector2 bul_pos = m_bullets[i]->GetPos();
		if (Vector2::Distance(m_pos, bul_pos)>3000)
		{
			m_bullets.erase(m_bullets.begin() + i);
			std::cout << "Erasing at " << i << "\n";
		}
	}
}


void Cannon::Set_Position(Vector2& _pos)
{
	m_pos.x = _pos.x;
	m_pos.y = _pos.y;
}

void Cannon::Set_Position(float _x, float _y)
{
	m_pos.x = _x;
	m_pos.y = _y;
}

const Vector2& Cannon::Get_Position()
{
	// TODO: insert return statement here
	return m_pos;
}

//The idea is we create the objects somewhere externally and dispense them 
//with this function, make sure to reference the created object to null above
// this class will deal with deleteing these objects.
void Cannon::Set_Objects(Object* _barrel, Object* _foot_hold)
{
	//Delete old objects if any
	if (m_barrel != NULL)
	{
		delete m_barrel;
		m_barrel = NULL;
	}
	if(m_foot_hold != NULL)
	{
		delete m_foot_hold;
		m_foot_hold = NULL;
	}

	m_barrel = _barrel;
	m_foot_hold = _foot_hold;


}
void Cannon::Shoot()
{
	Vector2 adjusted_pos = m_pos;
	adjusted_pos.x -= m_barrel->m_Pos.x;
	adjusted_pos.y -= m_barrel->m_Pos.y;

	LazerProjectile* proj = new LazerProjectile(m_barrel_direction, m_barrel_direction, adjusted_pos);
	m_bullets.push_back(proj);
}

void Cannon::draw(SDLRenderer* _renderer)
{
	//World pos substracts normally, but in our player's case which is static
	// we want to add this position so the barrel and foothold have a "local position"
	Vector2 _inverse_pos = m_pos * -1;
	m_barrel->Render(_renderer, _inverse_pos,2);
	m_foot_hold->Render(_renderer, _inverse_pos,1);

	for (uint32_t i = 0; i < m_bullets.size(); ++i)
	{
		m_bullets[i]->Render(_renderer);
	}
	if (m_debug_mode)
	{
		//Directionline

		Line l;
		l.colour = {0xff,0x00,0x00,0xff};
		l.start = (m_barrel->m_Pos*-1);
		l.end = (m_barrel->m_Pos*-1) + (m_barrel_direction * 100);
		_renderer->AddLine(l, _inverse_pos,HDEFAULTEBUGLAYER);
	}

}

void Cannon::Rotate(float _by)
{
	m_rotation += _by;

	m_barrel_direction.x = cosf(m_rotation);
	m_barrel_direction.y = sinf(m_rotation);
	m_barrel_direction.Normalize();
}

void Cannon::DebugMode(bool _new_val)
{
	m_debug_mode = _new_val;
}

void Cannon::Init(Vector2& _pos)
{
	Set_Position(_pos);

	//Create default barrel, and foot_hold object using the assets
	m_barrel = new Object{ { -20,-60 }, {150, 150 }};
	m_foot_hold = new Object{ { 0,0 }, {100, 100} };

	ResourceManager* man = ManagerSingleton::getInstance().res_man;
	m_barrel->m_RenderInterface.texture = man->LoadTexture("Assets//SpriteSheets//player//lazer_barrel.png");
	m_foot_hold->m_RenderInterface.texture = man->LoadTexture("Assets//SpriteSheets//player//laser_base.png");
	
	//dereference resourcemanager 
	man = nullptr;

	//default values for rotation
	m_rotation, m_prev_rotation = 0.0f;
	m_barrel_direction = { 1,0 };//cosine of 0 = 1 and sine of 0 = 0
	m_bull_accel = 0.1f;

	m_debug_mode = false;

}
