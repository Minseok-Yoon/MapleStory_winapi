#pragma once
#include "CObject.h"

class CCollider;

class CItem :
    public CObject
{
    friend class CMeso;

public:
    CItem();
    ~CItem();

public:
    Vec2 m_vPos;

public:
    void SetItemInfo(const Vec2& _pos) { m_vPos = _pos; };
    Vec2 GetItemInfo() const { return m_vPos; }

public:
    virtual void Update();
    virtual void Render(HDC _dc);
    CLONE(CItem);
};