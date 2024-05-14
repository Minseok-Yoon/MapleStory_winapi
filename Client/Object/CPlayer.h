#pragma once
#include "CObject.h"

class CTexture;

class CPlayer :
    public CObject
{
public:
    CPlayer();
    virtual ~CPlayer();

private:
    CTexture*           m_pTex;
    vector<CObject*>    vecColObject;
    PLAYER_STATE        m_eCurState;    // 플레이어의 현재 상태
    PLAYER_STATE        m_ePrevState;   // 플레이어의 이전 상태
    int                 m_iDir;         // 플레이어의 방향
    int                 m_iPrevDir;     // 플레이어의 이전 방향

    Vec2                m_vFinalPos;

public:
    Vec2 GetFinalPos() { return m_vFinalPos; }

public:
    void Update_State();
    void Update_Move();

    virtual void OnCollisionEnter(CCollider* _pOther);

public:
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CPlayer);
};