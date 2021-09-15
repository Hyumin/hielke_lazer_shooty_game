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

}

void Cannon::Update(float _dt)
{
	m_cd_timer += _dt;
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
Projectile* Cannon::Shoot()
{
	if (m_cd_timer >= m_cd)
	{
		Vector2 adjusted_pos = m_pos + m_barrel->m_Pos;
		adjusted_pos.x += m_barrel->m_RenderInterface.point.x;
		adjusted_pos.y += m_barrel->m_RenderInterface.point.y;
		//Take width and height of the projectile /2 and multiply with direction asuming direction is a normalized direction vector
		printf("Barreldirection(%f,%f) \n", m_barrel_direction.x, m_barrel_direction.y);


		m_cd_timer = 0.00f;
		LazerProjectile* proj = new LazerProjectile(m_barrel_direction, m_barrel_direction * 1000, adjusted_pos);
		proj->m_debug = m_debug_mode;
		proj->SetAngle(m_rotation);
		return proj;
	}
	return nullptr;
}

void Cannon::draw(SDLRenderer* _renderer)
{
	//World pos substracts normally, but in our player's case which is static
	// we want to add this position so the barrel and foothold have a "local position"
	Vector2 _inverse_pos = m_pos * -1;
	m_barrel->Render(_renderer, _inverse_pos,2);
	m_foot_hold->Render(_renderer, _inverse_pos,1);
	m_barrel->m_RenderInterface.angle = m_rotation* 57.32484076433121;
	if (m_debug_mode)
	{
		//Directionline

		Line l;
		Vector2 adjusted_pos = m_pos + m_barrel->m_Pos;
		adjusted_pos.x += m_barrel->m_RenderInterface.point.x;
		adjusted_pos.y += m_barrel->m_RenderInterface.point.y;
		l.colour = {0xff,0x00,0x00,0xff};
	
		l.start = (adjusted_pos);
		l.end = (adjusted_pos) + (m_barrel_direction * 1000);
		_renderer->AddLine(l, { 0,0 }, HDEFAULTEBUGLAYER);
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

	//Hardcoded point of the barrel, this point differs per barrel, maybe think about 
	// an external file to load in the barrel objec in the future
	SDL_Point p;
	p.x = 69;
	p.y = 96;
	m_barrel->m_RenderInterface.point = p;
	
	//dereference resourcemanager 
	man = nullptr;

	//default values for rotation
	m_rotation, m_cd_timer, m_prev_rotation = 0.0f;
	m_barrel_direction = { 1,0 };//cosine of 0 = 1 and sine of 0 = 0
	m_debug_mode = false;

	m_cd = 0.1f;

}
