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

	Vec2(const Vec2& _pos) :
		x(_pos.x),
		y(_pos.y)
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

		return *this;
	}

	Vec2 operator = (const Vec2& _pos)
	{
		x = _pos.x;
		y = _pos.y;

		return *this;
	}

	Vec2& operator = (float f[2])
	{
		x = f[0];
		y = f[1];
		return *this;
	}

	Vec2 operator + (const Vec2& _vOther) const
	{
		return Vec2(x + _vOther.x, y + _vOther.y);
	}

	Vec2 operator + (const POINT& _pt) const
	{
		return Vec2(x + _pt.x, y + _pt.y);
	}

	Vec2 operator + (float f[2]) const
	{
		return Vec2(x + f[0], y + f[1]);
	}

	Vec2 operator + (float _f) const
	{
		return Vec2(x + _f, y + _f);
	}

	void operator += (const Vec2 _vOther)
	{
		x += _vOther.x;
		y += _vOther.y;
	}

	void operator += (const POINT& _pt)
	{
		x += _pt.x;
		y += _pt.y;
	}

	void operator += (float f[2])
	{
		x += f[0];
		y += f[1];
	}

	void operator += (float _f)
	{
		x += _f;
		y += _f;
	}

	// ==========================================
	Vec2 operator - ()
	{
		return Vec2(-x, -y);
	}

	Vec2 operator - (const Vec2& _vOther) const
	{
		return Vec2(x - _vOther.x, y - _vOther.y);
	}

	Vec2 operator - (const POINT& _pt) const
	{
		return Vec2(x - _pt.x, y - _pt.y);
	}

	Vec2 operator - (float f[2]) const
	{
		return Vec2(x - f[0], y - f[1]);
	}

	Vec2 operator - (float _f) const
	{
		return Vec2(x - _f, y - _f);
	}

	void operator -= (const Vec2& _vOther)
	{
		x -= _vOther.x;
		y -= _vOther.y;
	}

	void operator -= (const POINT& _pt)
	{
		x -= _pt.x;
		y -= _pt.y;
	}

	void operator -= (float f[2])
	{
		x -= f[0];
		y -= f[1];
	}

	void operator -= (float f)
	{
		x -= f;
		y -= f;
	}

	Vec2 operator * (const Vec2& _vOther) const
	{
		return Vec2(x * _vOther.x, y * _vOther.y);
	}

	Vec2 operator * (const POINT& _pt) const
	{
		return Vec2(x * _pt.x, y * _pt.y);
	}

	Vec2 operator * (float f[2]) const
	{
		return Vec2(x * f[0], y * f[1]);
	}

	Vec2 operator * (float f) const
	{
		return Vec2(x * f, y * f);
	}

	void operator *= (const Vec2& _vOther)
	{
		x *= _vOther.x;
		y *= _vOther.y;
	}

	void operator *= (const POINT& _pt)
	{
		x *= _pt.x;
		y *= _pt.y;
	}

	void operator *= (float f[2])
	{
		x *= f[0];
		y *= f[1];
	}

	void operator *= (float f)
	{
		x *= f;
		y *= f;
	}

	Vec2 operator / (const Vec2& _vOther) const
	{
		return Vec2(x / _vOther.x, y / _vOther.y);
	}

	Vec2 operator / (const POINT& _pt) const
	{
		return Vec2(x / _pt.x, y / _pt.y);
	}

	Vec2 operator / (float f[2]) const
	{
		return Vec2(x / f[0], y / f[1]);
	}

	Vec2 operator / (float f) const
	{
		assert(f != 0.f);
		return Vec2(x / f, y / f);
	}

	void operator /= (const Vec2& _vOther)
	{
		x /= _vOther.x;
		y /= _vOther.y;
	}

	void operator /= (const POINT& _pt)
	{
		x /= _pt.x;
		y /= _pt.y;
	}

	void operator /= (float f[2])
	{
		x /= f[0];
		y /= f[1];
	}

	void operator /= (float f)
	{
		assert(f != 0.f);
		x /= f;
		y /= f;
	}
};

typedef struct _tagResolution
{
	unsigned int	iW;
	unsigned int	iH;

	// 생성자 오버로딩
	_tagResolution() :
		iW(0),
		iH(0)
	{
	}

	_tagResolution(int x, int y) :
		iW(x),
		iH(y)
	{
	}
}RESOLUTION, * PRESOLUTION;

typedef struct _tagRectangle
{
	float	l;
	float	t;
	float	r;
	float	b;

	_tagRectangle() :
		l(0.f),
		t(0.f),
		r(0.f),
		b(0.f)
	{
	}
}RECTANGLE, * PRECTANGLE;

typedef struct _tagPixel
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}PIXEL, * PPIXEL;