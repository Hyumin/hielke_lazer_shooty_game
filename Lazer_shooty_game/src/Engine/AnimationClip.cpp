#include "AnimationClip.h"

#include "ResourceManager.h"
#include "Texture.h"
#include "..\HielkMath.h"

AnimationClip::AnimationClip()
{
	m_AnimInterval = 0.5f;
	m_AnimTimer = 0.0f;
	m_CurrentIndex = 0;
	m_IsPlaying = false;
	m_Looping = false;
	m_ClipName = " ";
	m_IsFinished = false;
	m_SourceTexture = NULL;
}

AnimationClip::~AnimationClip()
{
}

void AnimationClip::Update(float _dt)
{
	if (m_SourceRects.size() > 0)
	{
		if (m_IsPlaying)
		{
			//Add time to the timer
			m_AnimTimer += _dt;
			if (m_AnimTimer > m_AnimInterval)
			{
				//increment the index and check whether its bigger than the size of the array
				m_AnimTimer -= m_AnimInterval;
				m_CurrentIndex++;
				if (m_CurrentIndex > 100)
				{
					printf("nandayo \n");
				}
				if (m_CurrentIndex >= m_SourceRects.size())
				{
					m_CurrentIndex = 0;
					//If we're not looping stop playing
					if (!m_Looping)
					{
						m_IsPlaying = false;
						m_IsFinished = true;
					}
				}
			}

		}
	}
}

SDL_Rect& AnimationClip::GetRect()
{
	if (m_SourceRects.size() == 0)
	{
		throw std::exception(("Animation clip with filename"+m_FileName+" has no clips | Animationlip.cpp \n").c_str());
	}

	return m_SourceRects[m_CurrentIndex];
}


Vector2 AnimationClip::GetOffset()
{
	Vector2 defaultReturnValue = { 0,0 };
	if (m_UseOffsets)
	{	
		if (m_Offsets.size() > 0)
		{
			return m_Offsets[m_CurrentIndex];
		}
		else
		{
			return defaultReturnValue;
		}
	}
	return defaultReturnValue;
}

void AnimationClip::LoadClipFromFile(const std::string& _path, ResourceManager* _manager)
{
	m_SourceRects.clear();
	m_SourceTexture = nullptr;

	std::string stuff = " ";
	std::ifstream file(_path);
	m_FileName = _path;

	while (!file.eof())
	{

		file >> stuff;
		if (stuff == "Texture:")
		{
			file >> stuff;
			m_SourceTexture = _manager->GetTexture(stuff);
		}
		if (stuff == "Clip_name:")
		{
			file >> m_ClipName;
		}
		if (stuff == "SDL_rect:")
		{
			int x, y, w, h;
			file >> x;
			file >> y;
			file >> w;
			file >> h;
			SDL_Rect newRect = { x, y, w, h };
			m_SourceRects.push_back(newRect);
		}
		if (stuff == "Using_offset:")
		{
			file >> m_UseOffsets;
		}
		if (stuff == "Offset:")
		{
			Vector2 offset;
			file >> offset.x >> offset.y;
			m_Offsets.push_back(offset);
		}
		if (stuff == "is_looping:")
		{
			file >> m_Looping;
		}
		if (stuff == "anim_interval:")
		{
			file >> m_AnimInterval;
		}

	}
	GenerateOffsetsArray();
}

void AnimationClip::SaveClipToFilePath(const std::string& _path)
{
	std::ofstream file (_path);

	//Variable to keep track of which source rect or offsets we are
	unsigned int index = 0;
	file << std::string("Texture: ") + m_SourceTexture->GetName() + '\n';
	file << "Clip_name: " + m_ClipName + '\n';
	file << "anim_interval: " + std::to_string(m_AnimInterval) + '\n';
	file << "is_looping: " << m_Looping << '\n';
	file << "Using_offset: " << m_UseOffsets << '\n';
	while (index < m_SourceRects.size())
	{
		SDL_Rect r = m_SourceRects[index];
		file <<"SDL_rect: "<< r.x << " " << r.y<< " "<< r.w<< " "<< r.h << '\n' ;
		if (m_UseOffsets)
		{
			Vector2 v = m_Offsets[index];
			file << "Offset: " << v.x << " " << v.y << "\n";
		}


		index++;
	}

	file.close();
}

void AnimationClip::Play()
{
	m_IsPlaying = true;
	m_IsFinished = false;
}

void AnimationClip::NextFrame()
{
	if (m_CurrentIndex < m_SourceRects.size()-1)
	{
		m_CurrentIndex++;
	}
	else
	{
		m_CurrentIndex = 0;
	}
}
void AnimationClip::PrevFrame()
{
	if (m_CurrentIndex > 0)
	{
		m_CurrentIndex--;
	}
	else
	{
		if (m_SourceRects.size() > 0)
		{
			m_CurrentIndex = (unsigned int)m_SourceRects.size() - 1;
		}
	}
}

void AnimationClip::GenerateOffsetsArray()
{
	if (m_SourceRects.size() != 0)
	{
		for (int i = (int)m_Offsets.size(); i < m_SourceRects.size(); ++i)
		{
			m_Offsets.push_back({});
		}
	}

}

void AnimationClip::AddFrameAtIndex(unsigned int _index, SDL_Rect _rect)
{
	//Smaller or equals will allow it to add a frame at the end!
	if (_index <= m_SourceRects.size())
	{
		m_SourceRects.insert(m_SourceRects.begin() + _index, _rect);
		m_Offsets.insert(m_Offsets.begin() + _index, { 0,0 });
	}
	if (m_SourceRects.size() == 0)
	{
		m_SourceRects.push_back(_rect);
		m_Offsets.push_back({ 0,0 });
	}
}

void AnimationClip::RemoveFrameAtIndex(unsigned int _index)
{
	if (m_SourceRects.size() == 1)
	{
		m_SourceRects.clear();
		m_Offsets.clear();
	}
	if (_index < m_SourceRects.size())
	{
		m_SourceRects.erase(m_SourceRects.begin() + _index);
		m_Offsets.erase(m_Offsets.begin() + _index);
	}
	
}



