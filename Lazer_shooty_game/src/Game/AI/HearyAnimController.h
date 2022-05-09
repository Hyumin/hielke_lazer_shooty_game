class AnimationClip;
class Heary;

#include <unordered_map>
//first we'lle create a speicif controller later on this should inherit from a generic base class
// the baseclass should contain only easy loading functions for animationclips
// and a unsigned int or char for the key so we can define every state with an animation or smthing.
class HearyAnimController
{
public:
	enum AnimState
	{
		Moving = 0
	};


	HearyAnimController();
	~HearyAnimController();
	void Update(float _dt);
	void TriggerEvent(unsigned int _input);
	bool LoadClips();
	void HandleMovingState(float _dt);
	void SetState(AnimState _state);
	AnimationClip* GetClip();

	void SetOwner(Heary* _arg);
	const Heary* GetOwner() { return m_owner; }

private:

	Heary* m_owner = nullptr; // reference to the heary that owns this controller/statemachine

	AnimState m_state = Moving;
	
	bool m_falling = false;
	std::unordered_map<AnimState, AnimationClip*> m_clips;
	AnimationClip* m_current_clip;

};