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
    PLAYER_STATE        m_eCurState;    // �÷��̾��� ���� ����
    PLAYER_STATE        m_ePrevState;   // �÷��̾��� ���� ����
    int                 m_iDir;         // �÷��̾��� ����
    int                 m_iPrevDir;     // �÷��̾��� ���� ����

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