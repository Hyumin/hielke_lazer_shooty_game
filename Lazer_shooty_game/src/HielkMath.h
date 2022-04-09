#pragma once
#include <math.h>
#include <exception>

//header file containing classes for all of our math operations and functions etc.

class Vector2
{
public:
	Vector2() { x = 0.0f; y = 0.0f; }
	Vector2(float _x, float _y) { x = _x; y = _y; }
	~Vector2() {}

	void operator= ( const Vector2& rhand)
	{
		x = rhand.x;
		y = rhand.y;
	}
	Vector2 operator+ (const Vector2& rhand) {
		
		Vector2 newVec;
		newVec.x = rhand.x + x;
		newVec.y = rhand.y + y;
		return newVec;
	}
	Vector2 operator- (const Vector2& rhand)
	{
		Vector2 newVec;
		newVec.x = x - rhand.x;
		newVec.y = y - rhand.y;
		return newVec;
	}

	Vector2 operator* (const float& rhand)
	{
		Vector2 newVec;
		newVec.x = x * rhand;
		newVec.y = y * rhand;
		return newVec;
	}
	Vector2 operator* (const float& rhand) const
	{
		Vector2 newVec;
		newVec.x = x * rhand;
		newVec.y = y * rhand;
		return newVec;
	}
	Vector2 operator/ (const float& rhand)
	{
		Vector2 newVec;
		newVec.x = x / rhand;
		newVec.y = y / rhand;
		return newVec;
	}
	void operator/= (const float& rhand)
	{
		x = x / rhand;
		y = y / rhand;
	}

	void operator*= (float rhand)
	{
		x *= rhand;
		y *= rhand;
	}

	void operator+= (const Vector2& rHand)
	{
		x += rHand.x;
		y += rHand.y;
	}
	void operator-= (const Vector2& rHand)
	{
		x -= rHand.x;
		y -= rHand.y;
	}
	bool operator != (const Vector2& rHand)
	{
		if (rHand.x != x)
		{
			return false;
		}
		if (rHand.y != y)
		{
			return false;
		}

		return true;
	}

	float Dot(Vector2& rHand)
	{
		return x * rHand.x + y * rHand.y;
	}
	float Dot(Vector2& l, Vector2& r)
	{
		return l.x * r.x + l.y * r.y;
	}

	float Magnitude()
	{
		return sqrtf((x*x)+(y*y));
	}
	static float Distance(Vector2& _a, Vector2& _b)
	{
		float result = 0.0f;
		Vector2 vec = _a - _b;
		result = vec.Magnitude();
		return result;
	}
	void Normalize()
	{
		float mag = Magnitude();
		if (mag == 0)
		{
			x = 0;
			y = 0;
			return;
		}
		x /= mag;
		y /= mag;
	}


	float x, y;
private:

};

//Although rectangle is a much better way to describe it 
//using box to differentiate from SDL_rect
class Box
{
public:
	Box()
	{
		pos.x = 0.0f;
		pos.y = 0.0f;
		w = 1.0f;
		h = 1.0f;
	}
	Box(const Vector2& _pos, float _w,float _h)
	{
		pos = _pos;
		w = _w;
		h = _h;
	}
	Box(float _x, float _y, float _w, float _h) 
	{
		pos.x = _x;
		pos.y = _y;
		w = _w;
		h = _h;
	}
	/*Box(int _x, int _y, int _w, int _h)
	{
		pos.x = (float)_x;
		pos.y = (float)_y;
		w = (float)_w;
		h = (float)_h;
	}*/
	~Box(){}

	static bool BoxCollision(const Box& _b1, const Box& _b2)
	{

		if (_b1.pos.x + _b1.w >= _b2.pos.x &&
			_b1.pos.y + _b1.h >= _b2.pos.y &&
			_b1.pos.x <= _b2.pos.x + _b2.w &&
			_b1.pos.y <= _b2.pos.y + _b2.h)
		{
			return true;
		}

		return false;
	}
	static bool BoxCollision(const Box& _b1, const Vector2 _p1)
	{
		if (_b1.pos.x + _b1.w >= _p1.x &&
			_b1.pos.y + _b1.h >= _p1.y &&
			_b1.pos.x <= _p1.x &&
			_b1.pos.y <= _p1.y)
		{
			return true;
		}


		return false;
	}


	Vector2 pos;
	float w, h;
private:

};

class CircleCollider
{
public:
	CircleCollider() {
		pos = Vector2();
		radius = 1.0f;
	}
	~CircleCollider() {
		
	}


	Vector2 pos;
	float radius;
};

//For collision with a point we will assume a radius of 0.1 for the "point"
static bool CircleCollisionPoint(CircleCollider& _a ,Vector2 _b)
{
	return Vector2::Distance(_a.pos, _b) <= _a.radius + 0.1f; 
}

static bool CircleToCircleCollision(CircleCollider& _a, CircleCollider& _b)
{
	return Vector2::Distance(_a.pos, _b.pos) <= _a.radius + _b.radius;
}

static Vector2 ClosestPtPointAABB(Vector2 _p, Box _box)
{
	Vector2 return_val;


	float v = _p.x;
	if (v < _box.pos.x) v = _box.pos.x;
	if (v > _box.pos.x + _box.w) v = _box.pos.x + _box.w;
	return_val.x = v;

	v = _p.y;
	if (v < _box.pos.y) v = _box.pos.y;
	if (v > _box.pos.y + _box.h) v = _box.pos.y + _box.h;
	return_val.y = v;

	return return_val;

}

static bool CircleToBoxCollision(CircleCollider& _a, Box& _b)
{
	Vector2 closest = ClosestPtPointAABB(_a.pos, _b);

	return Vector2::Distance(_a.pos, closest) <= _a.radius;
}

static int random_range(int _min, int _max)
{
	if (_min > _max)
	{
		throw(std::exception("Can not execute this function if min is higher than the maximum :("));
	}

	int random_num = rand() % _max + _min;

	return random_num;
}

static float random_range(float _min, float _max)
{
	if (_min > _max)
	{
		throw(std::exception("Can not execute this function if min is higher than the maximum :("));
	}

	float random_num = _min + (float)(rand()) / ((float)RAND_MAX/_max);

	return random_num;
}

//TODO move everything to HielkMath, will cause ALL the classes to face compilation errors :)
namespace HielkMath
{
	static float clamp(float _input, float _min, float _max)
	{
		if (_min >= _input)
		{
			return _min;
		}
		if (_max <= _input)
		{
			return _max;
		}

		return _input;
	}
};