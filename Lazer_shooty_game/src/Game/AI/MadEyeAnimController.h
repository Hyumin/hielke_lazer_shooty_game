
class AnimationClip;
class MadEye;

#include <unordered_map>
//first we'lle create a speicif controller later on this should inherit from a generic base class
// the baseclass should contain only easy loading functions for animationclips
// and a unsigned int or char for the key so we can define every state with an animation or smthing.
class MadEyeAnimController
{
public:
	enum AnimState
	{
		Moving = 0, Falling = 1
	};


	MadEyeAnimController();
	~MadEyeAnimController();
	void Update(float _dt);
	void TriggerEvent(unsigned int _input);
	bool LoadClips();
	void HandleMovingState(float _dt);
	void SetState(AnimState _state);
	AnimationClip* GetClip();

	MadEye* m_owner = nullptr; // reference to the madeye that owns this controller/statemachine
	

private :


	AnimState m_state = Moving;
	void FallDown();

	bool m_falling = false;
	std::unordered_map<AnimState, AnimationClip*> m_clips;
	AnimationClip* m_current_clip;

	float m_fall_ocity =0.0f;
	float m_fal_speed = 1.0f;

};