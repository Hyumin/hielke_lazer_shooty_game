#include "MadEyeAnimController.h"
#include "MadEye.h"
#include <ManagerSingleton.h>
#include <AnimationClip.h>
#include <functional>

MadEyeAnimController::MadEyeAnimController()
{
}

MadEyeAnimController::~MadEyeAnimController()
{
}

void MadEyeAnimController::Update(float _dt)
{
	if (m_owner != nullptr)
	{
		m_current_clip->Update(_dt);
		Vector2 pos = m_owner->GetPosition();
		if (m_falling)
		{
			m_fall_ocity += _dt * m_fal_speed;
			pos.y += m_fall_ocity;
			if (pos.y > 1040)
			{
				m_current_clip->Resume();
			}
			else
			{
				m_owner->SetPosition(pos);
			}
		}

		switch (m_state)
		{
			case Moving:
				HandleMovingState(_dt);
				break;
			case Falling:
				if (m_current_clip->m_IsFinished)
				{
					m_owner->Die();
				}
				break;
		default:
			break;
		}
	}
}
void MadEyeAnimController::FallDown()
{
	m_falling = true;
	m_fall_ocity = 0.2f;
	
}

void MadEyeAnimController::TriggerEvent(unsigned int _input)
{
	printf("Triggering event: \n");
	switch (_input)
	{
		//Falldown
	case Falling:
		FallDown();
		printf("Falling");
		m_current_clip->Pause();
		break;
		//shoot
	case Moving:
		printf("Moving:");
		break;
	default:
		break;
	}
}

bool MadEyeAnimController::LoadClips()
{
	AnimationClip* move = new AnimationClip();
	AnimationClip* falling = new AnimationClip();
	move->LoadClipFromFile("Assets//AnimationClips//enemies//mad_eye//mad_eye_walk.hanimclip", ManagerSingleton::getInstance().res_man);
	falling->LoadClipFromFile("Assets//AnimationClips//enemies//mad_eye//mad_eye_death.hanimclip", ManagerSingleton::getInstance().res_man);
	unsigned int numby = 0;
	//std::bind(MadEyeAnimController::TriggerEvent, std::placeholders::_1)
	/*std::function<void(unsigned int)>*/ auto callback = std::bind(&MadEyeAnimController::TriggerEvent,this, std::placeholders::_1);
	move->SetFunction(callback);
	//Set function pointer in MOVE and Falling for the trigger event
	falling->SetFunction(callback);
	
	m_clips[AnimState::Moving] = move;
	m_clips[AnimState::Falling] = falling;
	return true;
}

void MadEyeAnimController::HandleMovingState(float _dt)
{
	m_current_clip->m_AnimInterval = (2 - m_owner->GetCurrSpeed() / 300.0f) * 0.15f;
}

void MadEyeAnimController::SetState(AnimState _state)
{
	m_current_clip = m_clips[_state];
	m_state = _state;
	//Assign clip and perhaps do extra stuff based on the state we're going to
	switch (_state)
	{
	case Moving:
		break;
	case Falling:
		printf("Setting state to falling \n");
		break;
	
	}
	//reset anim clip
	m_current_clip->Reset();
	m_current_clip->Play();


}

AnimationClip* MadEyeAnimController::GetClip()
{
	return m_current_clip;
}
