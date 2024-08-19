#include "../pch.h"
#include "CObjPixCollision.h"

CObjPixCollision::CObjPixCollision() :
	m_pPixelCollider(nullptr)
{
}

CObjPixCollision::~CObjPixCollision()
{
}

// �浹 ��ǥ ���
vector<Vec2> CObjPixCollision::GetCollisionPoints(const Vec2& _vPos, int _iPlayerWidthHalf, int _iPlayerHeightHalf)
{
    vector<Vec2> collisionPoints;       // �浹 ��ǥ�� ����
    int offsetY = _iPlayerHeightHalf;

    // ���� ��ġ�� ���� ��ǥ�� ���
    int x = static_cast<int>(_vPos.x);
    int y = static_cast<int>(_vPos.y + offsetY);

    // �¿� �浹 �˻� ��ǥ�� ���
    int xLeft = x - _iPlayerWidthHalf;
    int xRight = x + _iPlayerWidthHalf;

    // ���� �浹 �˻� ��ǥ�� ���
    int yTop = y - _iPlayerHeightHalf;
    int yBottom = y + _iPlayerHeightHalf;

    // �浹��ǥ�� ���͸� �߰�
    collisionPoints.emplace_back(Vec2(x, y));
    collisionPoints.emplace_back(Vec2(xLeft, y));
    collisionPoints.emplace_back(Vec2(xRight, y));
    collisionPoints.emplace_back(Vec2(x, yTop));

    return collisionPoints;
}

// �ȼ� �浹 ���θ� Ȯ��
bool CObjPixCollision::CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag)
{
    // ��ǥ�� �ȼ� �浹 ��ü�� ���� ���� �ִ��� Ȯ��
    if (_iPosX >= 0 && _iPosX < m_pPixelCollider->GetWidth() && _iPosY >= 0 && _iPosY < m_pPixelCollider->GetHeight()) {
        _pPixel = m_pPixelCollider->GetPixelColor(_iPosX, _iPosY);

        // �浹 �±׿� ���� �浹 ���θ� ����
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