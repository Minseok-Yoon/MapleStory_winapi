#pragma once
#include "../Component/CColliderPixel.h"

class CObjPixCollision
{
public:
	CObjPixCollision();
	~CObjPixCollision();

private:
	CColliderPixel* m_pPixelCollider;

public:
	void SetPixelCollider(CColliderPixel* _pPixelCollider) { m_pPixelCollider = _pPixelCollider; }

public:
	vector<Vec2>GetCollisionPoints(const Vec2& _vPos, int _iPlayerWidthHalf, int _iPlayerHeightHalf);
	bool CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag);
};