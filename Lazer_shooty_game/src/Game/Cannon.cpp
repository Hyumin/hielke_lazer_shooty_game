#include "Cannon.h"
#include "..\Engine\ManagerSingleton.h"
#include "..\Engine\AnimationClip.h"
#include "LazerProjectile.h"
#include "..\Engine\TextField.h"
#include "..\Engine\TextFieldBuilder.h"

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

	delete m_shoot_anim;
	m_shoot_anim = NULL;
	delete m_text_field;
	m_text_field = NULL;

}

void Cannon::Update(float _dt)
{
	m_cd_timer += _dt;
	if (m_shoot_anim != NULL)
	{
		m_shoot_anim->Update(_dt);

		m_shoot_anim->m_AnimInterval = 0.15f *m_stats.m_Proj_cd;
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

void Cannon::MouseMove(Vector2 _mpos)
{
	if (m_mouse_mode)
	{
		//Calculate the direction from the barrel to the mouse position and use this as dir
		Vector2 target_vec = (_mpos - m_pos);
		target_vec.Normalize();
		m_barrel_direction = target_vec;

		
		float rot = acosf(target_vec.x);
		if (target_vec.y < 0)
		{
			rot *= -1;
		}
		m_rotation = rot;

	}

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
	if (m_cd_timer >= m_stats.m_Proj_cd)
	{
		Vector2 adjusted_pos = m_pos + m_barrel->m_Pos;
		adjusted_pos.x += m_barrel->m_RenderInterface.point.x;
		adjusted_pos.y += m_barrel->m_RenderInterface.point.y;
		//Take width and height of the projectile /2 and multiply with direction asuming direction is a normalized direction vector

		m_cd_timer -= m_stats.m_Proj_cd;
		LazerProjectile* proj = new LazerProjectile(m_barrel_direction, m_barrel_direction * m_stats.m_Proj_speed*100, adjusted_pos);
		proj->Scale(m_stats.m_Proj_size);
		proj->m_debug = m_debug_mode;
		proj->SetAngle(m_rotation);
		if (m_shoot_anim != NULL)
		{
			if (m_shoot_anim->m_IsPlaying)
			{
				m_shoot_anim->m_CurrentIndex = 2;
			}
			m_shoot_anim->Play();
		}

		return proj;
	}
	return nullptr;
}

void Cannon::draw(SDLRenderer* _renderer)
{
	//World pos substracts normally, but in our player's case which is static
	// we want to add this position so the barrel and foothold have a "local position"
	Vector2 _inverse_pos = m_pos * -1;
	if (m_shoot_anim != NULL)
	{
		m_barrel->m_RenderInterface.srcRect = m_shoot_anim->GetRect();
		//
	}
	std::string textu = "LVL:" + std::to_string(m_stats.m_character_level) + "|Exp:" + std::to_string((int)m_stats.m_Current_Exp) + "/" + std::to_string((int)m_stats.m_Target_Exp);
	m_text_field->SetText(textu);
	m_text_field->Render(_renderer, { 0,0 }, 4);
	

	m_barrel->Render(_renderer, _inverse_pos,2);
	m_foot_hold->Render(_renderer, _inverse_pos,1);
	m_barrel->m_RenderInterface.angle = m_rotation* 57.32484076433121;
	_renderer->DrawFilledBox(0, 0, 600, 25, { 10,10,10,255 }, {0, 0}, 3);
	_renderer->DrawFilledBox(0, 0, 600 * (m_stats.m_Current_Exp / m_stats.m_Target_Exp), 25, { 183,255,40,255 }, { 0,0 },3);


	//if (m_debug_mode)
	//{
		//Directionline
		Line l;
		Vector2 adjusted_pos = m_pos + m_barrel->m_Pos;
		adjusted_pos.x += m_barrel->m_RenderInterface.point.x;
		adjusted_pos.y += m_barrel->m_RenderInterface.point.y;
		l.colour = {0xff,0x00,0x00,0xff};
	
		l.start = (adjusted_pos);
		l.end = (adjusted_pos) + (m_barrel_direction * 10000);
		_renderer->AddLine(l, { 0,0 }, HDEFAULTEBUGLAYER);
	//}

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

void Cannon::ToggleMouseMode()
{
	m_mouse_mode = m_mouse_mode ? false : true;
}

void Cannon::Init(Vector2& _pos)
{
	Set_Position(_pos);

	//Create default barrel, and foot_hold object using the assets
	m_barrel = new Object{ { -30,-90 }, {200, 160 }};
	m_foot_hold = new Object{ { 0,0 }, {100, 100} };

	ResourceManager* man = ManagerSingleton::getInstance().res_man;
	m_barrel->m_RenderInterface.texture = man->LoadTexture("Assets//SpriteSheets//player//lazer_barrel2.png");
	m_foot_hold->m_RenderInterface.texture = man->LoadTexture("Assets//SpriteSheets//player//laser_base.png");

	m_shoot_anim = new AnimationClip();

	m_shoot_anim->LoadClipFromFile("Assets//AnimationClips//cannon_barrel_shoot", ManagerSingleton::getInstance().res_man);
	//Hardcoded point of the barrel, this point differs per barrel, maybe think about 
	// an external file to load in the barrel objec in the future
	SDL_Point p;
	p.x = 80;
	p.y = 105;
	m_barrel->m_RenderInterface.point = p;
	m_barrel->m_RenderInterface.srcRect = { 0,0,32,32 };
	
	//dereference resourcemanager 
	man = nullptr;

	//default values for rotation
	m_rotation, m_cd_timer, m_prev_rotation = 0.0f;
	m_barrel_direction = { 1,0 };//cosine of 0 = 1 and sine of 0 = 0
	m_debug_mode = false;
	std::string textu = "LVL:" + std::to_string(m_stats.m_character_level) + "|Exp:" + std::to_string(m_stats.m_Current_Exp) + "/" + std::to_string(m_stats.m_Target_Exp);
	m_text_field = TextFieldBuilder::BuildTextFieldPtr(SDL_Colour{ 100,255,100,0 }, textu, ManagerSingleton::getInstance().res_man->GetFont("Assets//Fonts//Jupiter.ttf"), {0,26}, {600,40});
	
	m_cd = 0.1f;

}
