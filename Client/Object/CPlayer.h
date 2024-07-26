#pragma once
#include "CObject.h"

class CGravity;

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

    // Pixel
    PIXEL   m_CollisionPixelColor;
    bool                m_bStageCollision;

    // Rope ����
    bool                m_bRopeCollision;
    Vec2                m_vRopePos;

    CTexture* m_pTex;
    CColliderPixel* m_pPixelCollider;
    vector<Vec2> m_CollisionPoint;

public:
    void SetPixelCollider(CColliderPixel* _pPixelCollider) { m_pPixelCollider = _pPixelCollider; }

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

    // �浹ó�� �ݹ�
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

    CGravity* GetGravity() { return m_pGravity; }

public:
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CPlayer);
};