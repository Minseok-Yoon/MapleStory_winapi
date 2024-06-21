#pragma once
#include "CObject.h"

class CColliderPixel;

class CBackGround :
    public CObject
{
public:
    CBackGround();
    virtual ~CBackGround();

private:
    CTexture*       m_pTexture;
    CColliderPixel* m_pPixelCollider;

public:
    CColliderPixel* GetPixelCollider() const { return m_pPixelCollider; }

public:
    void Init(const wstring& _strKey);
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CBackGround);
};