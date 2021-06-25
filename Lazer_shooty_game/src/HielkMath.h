#pragma once
#include <math.h>

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

