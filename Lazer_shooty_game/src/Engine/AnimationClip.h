#pragma once

#include <SDL.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
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
	void LoadClipFromFile(const std::string& _path,ResourceManager* _manager);
	void SaveClipToFilePath(const std::string& _path);
	
	void Play();
	void NextFrame();
	void PrevFrame();
	void GenerateOffsetsArray();//Generate vector2.zero offsets to fill the array
	void AddFrameAtIndex(unsigned int _index, SDL_Rect _rect);
	void RemoveFrameAtIndex(unsigned int _index);

	std::vector<SDL_Rect> m_SourceRects;
	std::vector<Vector2> m_Offsets;
	Texture* m_SourceTexture;//what texture does this clip belong to
	unsigned int m_CurrentIndex;
	float m_AnimInterval;
	bool m_IsPlaying;
	bool m_Looping;
	bool m_IsFinished;
	bool m_UseOffsets = false;

	std::string m_ClipName;
	std::string m_FileName;

private:
	float m_AnimTimer;
	
};

