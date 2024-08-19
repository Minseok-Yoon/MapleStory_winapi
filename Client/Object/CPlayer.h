#pragma once
#include "CObject.h"

class CGravity;
class CMonster;

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
    
    bool                m_bJumpCycle;

    int                 m_iWallCollisionCount;
    bool                m_bLeftEnable;
    bool                m_bRightEnable;

    // Portal관련
    bool                m_bIsColPortal;
    wstring             m_strPortalTag; // 충돌한 포탈의 태그를 저장하는 변수

    // Attack관련
    CCollider* m_pRightAttackCollider;
    CCollider* m_pLeftAttackCollider;

    bool                m_bIsColMonster;
    bool                m_bAttackCycle;
    float m_fAttackDelayTime;  // 공격 후 대기 시간
    float m_fElapsedTime;      // 경과 시간

    // Pixel
    PIXEL   m_CollisionPixelColor;
    bool                m_bStageCollision;
    bool                m_bOnGround;

    // Rope 관련
    bool                m_bRopeCollision;
    bool                m_bRopeCycle;       // 로프에 매달려 있는 상태
    Vec2                m_vRopePos;

    CTexture* m_pTex;
    vector<Vec2> m_CollisionPoint;
    CColliderPixel* m_pPixelCollider;
    CMonster* m_pTargetMonster;

public:
    void SetPixelCollider(CColliderPixel* _pPixelCollider) { m_pPixelCollider = _pPixelCollider; }

public:
    void InitColliders();

public:
    void Update_State();
    void Update_Move();
    void Update_Animation();
    void PlayerAnimationClip();

    void AddAnimationClip(const wstring& _strKey, const wchar_t* _pFilePath, int _iFrameMax, float _fAnimationLimitTime, float _fFrameSizeX, float _fFrameSizeY);

    vector<Vec2>GetCollisionPoints(const Vec2& _vPos, int _iPlayerWidthHalf, int _iPlayerHeightHalf);
    bool CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag);
    void UpdateCollisionState(bool& _bIsColiding, bool _bCollisionDetected, const string& _strColTag, void (CPlayer::* onEnter)(), void (CPlayer::* onExit)());
    void CheckPixelColor();

    // 충돌처리 콜백
    void OnStageCollisionEnter();
    void OnStageCollisionExit();
    void OnWallCollisionEnter();
    void OnWallCollisionExit();
    void OnRopeCollisionEnter();
    void OnRopeCollisionExit();

    virtual void OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnCollision(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _ColTag, CCollider* _pOther);

    virtual void OnStageCollisionEnter(CCollider* _pOther);
    virtual void OnStageCollision(CCollider* _pOther);
    virtual void OnStageCollisionExit(CCollider* _pOther);

    virtual void OnWallCollisionEnter(CCollider* _pOther);
    virtual void OnWallCollision(CCollider* _pOther);
    virtual void OnWallCollisionExit(CCollider* _pOther);

    virtual void OnRopeCollisionEnter(CCollider* _pOther);
    virtual void OnRopeCollision(CCollider* _pOther);
    virtual void OnRopeCollisionExit(CCollider* _pOther);

    void PlayerAttack(CMonster* monster);

public:
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CPlayer);
};