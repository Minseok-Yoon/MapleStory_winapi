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
    PLAYER_STATE        m_eCurState;    // 플레이어의 현재 상태
    PLAYER_STATE        m_ePrevState;   // 플레이어의 이전 상태
    int                 m_iDir;         // 플레이어의 방향
    int                 m_iPrevDir;     // 플레이어의 이전 방향
    bool                m_bJump;

    int                 m_iWallCollisionCount;
    bool                m_bLeftEnable;
    bool                m_bRightEnable;

    // Portal관련
    bool                m_bIsColPortal;
    wstring             m_strPortalTag; // 충돌한 포탈의 태그를 저장하는 변수

    // Attack관련
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