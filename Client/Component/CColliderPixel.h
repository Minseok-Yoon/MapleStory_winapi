#pragma once
#include "CCollider.h"

class CColliderPixel :
    public CCollider
{
    friend class CObject;

public:
	CColliderPixel();
	CColliderPixel(const CColliderPixel& _origin);
	~CColliderPixel();

private:
	vector<PIXEL>	m_vecPixel;
	int				m_iWidth;
	int				m_iHeight;
	Vec2			m_vHitPoint;

public:
	const vector<PIXEL>& GetPixel()	const { return m_vecPixel; }

	int GetWidth()	const { return m_iWidth; }
	int GetHeight()	const {	return m_iHeight; }

	void SetHitPoint(const Vec2& _vHitPos) { m_vHitPoint = _vHitPos; }
	Vec2 GetHitPoint() const { return m_vHitPoint; }

public:
	// 픽셀 정보를 설정하는 함수
	bool SetPixelInfo(const wstring& _pFileName);
	PIXEL GetPixelColor(int x, int y);

public:
	virtual void FinalUpdate();
	virtual void Render(HDC _dc);
};