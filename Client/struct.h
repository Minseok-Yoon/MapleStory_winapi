#pragma once

struct Vec2
{
	float x;
	float y;

public:
	Vec2() :
		x(0.f),
		y(0.f)
	{}

	// 실수로 초기화 하는 경우
	Vec2(float _x, float _y) :
		x(_x),
		y(_y)
	{}

	// 정수로 초기화 하는 경우
	Vec2(int _x, int _y) :
		x(float(_x)),
		y(float(_y))
	{}

	Vec2(const POINT& _pt) :
		x((float)_pt.x),
		y((float)_pt.y)
	{}

public:
	bool IsZero()
	{
		if (x == 0.f && y == 0.f)
			return true;
		return false;
	}

	float Length()
	{
		return sqrt(x * x + y * y);
	}

	Vec2& Normalize()
	{
		float fLen = Length();

		assert(fLen != 0.f);

		x /= fLen;
		y /= fLen;

		return *this;
	}

public:
	Vec2& operator = (POINT& _pt)
	{
		x = (float)_pt.x;
		y = (float)_pt.y;
	}

	Vec2 operator + (const Vec2& _vOther)
	{
		return Vec2(x + _vOther.x, y + _vOther.y);
	}

	void operator += (const Vec2 _vOther)
	{
		x += _vOther.x;
		y += _vOther.y;
	}
	
	Vec2 operator - ()
	{
		return Vec2(-x, -y);
	}

	Vec2 operator -= (const Vec2& _vOther)
	{
		return Vec2(x -= _vOther.x, y -= _vOther.y);
	}

	Vec2 operator - (const Vec2& _vOther)
	{
		return Vec2(x - _vOther.x, y - _vOther.y);
	}

	Vec2 operator -= (float _vOther)
	{
		return Vec2(x -= _vOther, y -= _vOther);
	}

	Vec2 operator * (const Vec2& _vOther)
	{
		return Vec2(x * _vOther.x, y * _vOther.y);
	}

	Vec2 operator * (float _f)
	{
		return Vec2(x * _f, y * _f);
	}

	Vec2 operator / (float _f)
	{
		assert(!(0.f == _f));
		return Vec2(x / _f, y / _f);
	}
};