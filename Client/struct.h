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

	// 실수로 쵸기화 하는 경우
	Vec2(float _x, float _y) :
		x(_x),
		y(_y)
	{}

	// 정수로 쵸기화 하는 경우
	Vec2(int _x, int _y) :
		x((float)_x),
		y((float)_y)
	{}

};