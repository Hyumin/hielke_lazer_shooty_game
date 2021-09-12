#include <HielkMath.h>
#include <vector>

class EnemyPath
{
public:
	EnemyPath();
	~EnemyPath();

	Vector2 ProgressPath();//Will increment  the current point and return the new target point
	Vector2 ReturnCurrentPoint();
	void AddPoint(Vector2 _point);
	std::vector<Vector2> m_points;
	bool GetFinished()
	{
		return m_finished;
	}

private:
	unsigned int m_current_point;
	bool m_finished;
};