#pragma once
class Player_Stats
{
public:
	Player_Stats() {}
	~Player_Stats() {}

	//Abstract base class for stats for now we want to make exporting and loading in a mandatory thing to implement
	virtual int ExportStatsToFile(char* _filepath) = 0;
	virtual int LoadStatsFromFile(char* _filepath)= 0;

	//This function return which type of playerstats it is children should implement this so we can cast it to the proper class
	virtual int WhichClass() { return 0; }

	unsigned int m_character_level =0;

};


class Cannon_Stats:public Player_Stats
{
public:
	Cannon_Stats() { Init(); }
	~Cannon_Stats() {}


	void Init()
	{
		m_character_level= 1;
		m_Current_Exp = 0;
		m_Target_Exp = 1000;
	}

	void AddExp(float _exp)
	{
		m_Current_Exp += _exp;

		while (m_Current_Exp >= m_Target_Exp)
		{
			m_Current_Exp -= m_Target_Exp;
			LevelUp();
		}

	}

	//Not yet implemented returns -1 unsuccesfull
	virtual int ExportStatsToFile(char* _filepath) { return -1; }
	virtual int LoadStatsFromFile(char* _filepath) { return -1; }

	virtual int WhichClass()
	{
		return 1;
	}

	void LevelUp()
	{
		//For now sets level and increases every stat by 3%
		m_Proj_speed += 0.05f;
		m_Proj_dmg +=1.0f;
		m_Proj_size += 0.05f;
		m_Proj_cd -=0.01f;
		m_character_level += 1;
		m_Target_Exp += 500;
	}


	float m_Proj_speed = 1.0f;
	float m_Proj_dmg = 50.0f;
	float m_Proj_size = 1.0f;
	float m_Proj_cd = 0.5f;

	float m_Current_Exp;
	float m_Target_Exp;

};

