#pragma once
#include "CObject.h"

class CTexture;
class CColliderPixel;

class CPlayer :
    public CObject
{
public:
    CPlayer();
    virtual ~CPlayer();

private:
    PLAYER_STATE        m_eCurState;    // �÷��̾��� ���� ����
    PLAYER_STATE        m_ePrevState;   // �÷��̾��� ���� ����
    int                 m_iDir;         // �÷��̾��� ����
    int                 m_iPrevDir;     // �÷��̾��� ���� ����
    bool                m_bJump;

    int                 m_iWallCollisionCount;
    bool                m_bLeftEnable;
    bool                m_bRightEnable;

    // Portal����
    bool                m_bIsColPortal;
    wstring             m_strPortalTag; // �浹�� ��Ż�� �±׸� �����ϴ� ����

    // Attack����
    bool                m_bIsColMonster;

    CTexture*           m_pTex;
    CColliderPixel*     m_pPixelCollider;

public:
    void SetPixelCollider(CColliderPixel* _pPixelCollider) { m_pPixelCollider = _pPixelCollider; }

public:
    void Update_State();
    void Update_Move();
    void Update_Animation();
    void PlayerAnimationClip();

    void AddAnimationClip(const wstring& strKey, const wchar_t* pFilePath, int iFrameMax, float fAnimationLimitTime, float fFrameSizeX, float fFrameSizeY);
   
    void CheckPixelColor();

    void Attack();

    virtual void OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnCollision(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _ColTag, CCollider* _pOther);

    virtual void OnWallCollisionEnter(CCollider* _pOther);
    virtual void OnWallCollision(CCollider* _pOther);
    virtual void OnWallCollisionExit(CCollider* _pOther);

public:
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CPlayer);
};