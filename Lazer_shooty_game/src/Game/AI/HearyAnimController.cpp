#include "HearyAnimController.h"
#include "..\Engine\AnimationClip.h"
#include "ManagerSingleton.h"

HearyAnimController::HearyAnimController()
{
}

HearyAnimController::~HearyAnimController()
{
	delete m_current_clip;
}
/*
* Update the state machine, every state should have its own "Update" in which the clip is updated seperately 
*/
void HearyAnimController::Update(float _dt)
{
	switch (m_state)
	{
	case Moving:
		HandleMovingState(_dt);
		break;
	default:
		break;
	}
}

void HearyAnimController::TriggerEvent(unsigned int _input)
{
}

bool HearyAnimController::LoadClips()
{
	AnimationClip* walking = new AnimationClip();
	if (!walking->LoadClipFromFile("Assets//AnimationClips//enemies//mad_eye//heary_walk", ManagerSingleton::getInstance().res_man))
	{
		return false;
	}
	else
	{
		m_clips[AnimState::Moving] = walking;
	}

	//Set the current animationclip clip
	m_current_clip = m_clips[AnimState::Moving];
	m_current_clip->Play();

	return true;
}

void HearyAnimController::HandleMovingState(float _dt)
{
	m_current_clip->Update(_dt);

}

void HearyAnimController::SetState(AnimState _state)
{
	m_state = _state;
}

AnimationClip* HearyAnimController::GetClip()
{
	return m_current_clip;
}

void HearyAnimController::SetOwner(Heary* _arg)
{
	m_owner = _arg;
}

