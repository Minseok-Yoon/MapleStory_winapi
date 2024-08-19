#include "../pch.h"
#include "CObjPixCollision.h"

CObjPixCollision::CObjPixCollision() :
	m_pPixelCollider(nullptr)
{
}

CObjPixCollision::~CObjPixCollision()
{
}

// 충돌 좌표 계산
vector<Vec2> CObjPixCollision::GetCollisionPoints(const Vec2& _vPos, int _iPlayerWidthHalf, int _iPlayerHeightHalf)
{
    vector<Vec2> collisionPoints;       // 충돌 좌표를 저장
    int offsetY = _iPlayerHeightHalf;

    // 현재 위치의 정수 좌표로 계산
    int x = static_cast<int>(_vPos.x);
    int y = static_cast<int>(_vPos.y + offsetY);

    // 좌우 충돌 검사 좌표를 계산
    int xLeft = x - _iPlayerWidthHalf;
    int xRight = x + _iPlayerWidthHalf;

    // 상하 충돌 검사 좌표를 계산
    int yTop = y - _iPlayerHeightHalf;
    int yBottom = y + _iPlayerHeightHalf;

    // 충돌좌표에 벡터를 추가
    collisionPoints.emplace_back(Vec2(x, y));
    collisionPoints.emplace_back(Vec2(xLeft, y));
    collisionPoints.emplace_back(Vec2(xRight, y));
    collisionPoints.emplace_back(Vec2(x, yTop));

    return collisionPoints;
}

// 픽셀 충돌 여부를 확인
bool CObjPixCollision::CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag)
{
    // 좌표가 픽셀 충돌 객체의 범위 내에 있는지 확인
    if (_iPosX >= 0 && _iPosX < m_pPixelCollider->GetWidth() && _iPosY >= 0 && _iPosY < m_pPixelCollider->GetHeight()) {
        _pPixel = m_pPixelCollider->GetPixelColor(_iPosX, _iPosY);

        // 충돌 태그에 따라 충돌 여부를 판정
        if (_colTag == "StageColl") {
            return (_pPixel.r == 255 && _pPixel.g == 0 && _pPixel.b == 255);
        }
        else if (_colTag == "Wall") {
            return (_pPixel.r == 0 && _pPixel.g == 255 && _pPixel.b == 255);
        }
        else if (_colTag == "Rope") {
            return (_pPixel.r == 0 && _pPixel.g == 128 && _pPixel.b == 0);
        }
    }
    else {
        OutputDebugStringA("Player position out of bounds.\n");
    }
    return false;
}