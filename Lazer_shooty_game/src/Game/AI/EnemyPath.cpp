#include "EnemyPath.h"

EnemyPath::EnemyPath()
{
	m_current_point = 0;
	m_finished = false;
}

EnemyPath::~EnemyPath()
{

}

Vector2 EnemyPath::ProgressPath()
{
	//If we're finished we can just return the latest index.
	if (m_finished)
	{
		return m_points[m_current_point];
	}
	m_current_point += 1;
	//If we've reached beyond the size of our path array 
	if (m_current_point >= m_points.size())
	{
		//Set the route to finished and make sure the index is the LAST possible entry
		m_finished = true;
		m_current_point -= 1;
	}
	return m_points[m_current_point];
}

//This will return the current index in case we need it again for some sort of shenanigans
Vector2 EnemyPath::ReturnCurrentPoint()
{
	return m_points[m_current_point];
}

//Add point to the array
void EnemyPath::AddPoint(Vector2 _point)
{
	m_points.push_back(_point);
}
