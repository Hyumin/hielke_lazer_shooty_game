#pragma once

#include <SDL.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <functional>

class ResourceManager;
class Texture;
class Vector2;

class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();

	void Update(float _dt);
	SDL_Rect& GetRect();
	Vector2 GetOffset();
	bool LoadClipFromFile(const std::string& _path,ResourceManager* _manager);
	void SaveClipToFilePath(const std::string& _path);
	
	void Play();
	void Stop();
	void Pause();
	void Resume();
	void Reset();
	void NextFrame();
	void TriggerEvent();
	void PrevFrame();
	void SetFunction(std::function<void(unsigned int) > _func);
	void GenerateOffsetsArray();//Generate vector2.zero offsets to fill the array
	void AddFrameAtIndex(unsigned int _index, SDL_Rect _rect);
	void RemoveFrameAtIndex(unsigned int _index);

	std::vector<SDL_Rect> m_SourceRects;
	std::vector<Vector2> m_Offsets;
	Texture* m_SourceTexture;//Source texture this animationclip uses
	unsigned int m_CurrentIndex;
	float m_AnimInterval;
	bool m_IsPlaying;
	bool m_Looping;
	bool m_IsFinished;
	bool m_UseOffsets = false;
	bool m_UseEvents = false;

	std::unordered_map<unsigned int, unsigned int> m_Events; //  ( frame , event to trigger
	std::function<void(unsigned int)> m_IntCallback; // map to collect int callbacks

	std::string m_ClipName;
	std::string m_FileName;

private:
	float m_AnimTimer;
	
};

