#include "PlayerDebugInfo.h"
#include "..\Engine\TextFieldBuilder.h"
#include "..\Engine\AnimationClip.h"
#include <iomanip>
#include <sstream>

Hielke::PlayerDebugInfo::PlayerDebugInfo()
{
	m_Background = {0,0,400,300};
}

Hielke::PlayerDebugInfo::~PlayerDebugInfo()
{

}

void Hielke::PlayerDebugInfo::InitTextFields(TTF_Font* _font)
{
	m_anim = TextFieldBuilder::BuildTextField(m_TextColour, "AnimationState:", _font, { 0,0 }, { m_Background.w,30 });
	m_state = TextFieldBuilder::BuildTextField(m_TextColour, "PlayerState:", _font, { 0,30 }, { m_Background.w,30 });
	m_pos = TextFieldBuilder::BuildTextField(m_TextColour, "Position:", _font, { 0,60 }, { m_Background.w,30 });
	m_vel = TextFieldBuilder::BuildTextField(m_TextColour, "Velocity:", _font, { 0,90 }, { m_Background.w,30 });

	
	m_TextFields.push_back(&m_anim);
	m_TextFields.push_back(&m_state);
	m_TextFields.push_back(&m_pos);
	m_TextFields.push_back(&m_vel);
}

void Hielke::PlayerDebugInfo::CleanUp()
{
}

void Hielke::PlayerDebugInfo::Update(float _dt)
{
	if (m_Active)
	{
		//if textfields is bigger then 0 we have initialzed our textfields
		if (m_TextFields.size() > 0)
		{
		
		}
	}
}



void Hielke::PlayerDebugInfo::Toggle()
{
	m_Active =  m_Active ? false : true;
	//if any additional intialzitions is required put it in here
}

void Hielke::PlayerDebugInfo::Render(SDLRenderer* _renderer)
{
	if (m_Active)
	{
		Vector2 zerovec = { 0,0 };
		//render our textfields and box
		_renderer->DrawFilledBox(m_Background, m_BackgroundColour, zerovec, HDEFAULTEBUGLAYER);

		for (unsigned int i = 0; i < m_TextFields.size(); ++i)
		{
			m_TextFields[i]->Render(_renderer, zerovec, HDEFAULTEBUGLAYER);
		}
	}
}
