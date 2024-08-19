#pragma once
#include "CObject.h"

class AI;
class CTexture;
class CColliderPixel;

struct tMonInfo
{
    string  strTag;    // �̸�
    float   fSpeed;     // �ӵ�
    float   fHP;        // ü��
    float   fRecoRange; // ���� ����
    float   fAttRange;  // ���� ����
    float   fAtt;       // ���ݷ�
};

class CMonster :
    public CObject
{
    friend class CMonFactory;
    friend class CTraceState;
    friend class CPlayer;

public:
    CMonster(const Vec2& initialPos);
    virtual ~CMonster();

private:
    MON_STATE   m_eCurMonState;
    MON_STATE   m_ePrevMonState;
    CTexture* m_pTex;
    Vec2        m_vCenterPos;
    float       m_fSpeed;
    float       m_fMaxDistance;
    int         m_iDir;
    int         m_iPrevDir;

    int         m_iHP = 5;

    bool        m_bDeadAniFin;

    bool    bIsCollStage;
    bool    bIsCollWall;
    bool    bIsCollStageEnd;
    vector<Vec2> m_CollisionPoint;

    int                 m_iWallCollisionCount;
    bool                m_bLeftEnable;
    bool                m_bRightEnable;

    CColliderPixel* m_pPixelCollider;
    tMonInfo    m_tMonInfo;
    AI* m_pAI;

private:
    float m_fIdleTime; // ��� �ð��� �����ϴ� ����
    bool m_bWaiting; // ��� ������ ���θ� ��Ÿ���� ����

public:
    void    SetMonTag(string _strTag) { m_tMonInfo.strTag = _strTag; }
    string  GetMonTag() { return m_tMonInfo.strTag; }

    void    SetSpeed(float _fSpeed) { m_tMonInfo.fSpeed = _fSpeed; }
    float   GetSpeed() { return m_tMonInfo.fSpeed; }

    void    SetMoveDistance(float _fDist) { m_fMaxDistance = _fDist; }

    void    SetCenterPos(Vec2 _vPos) { m_vCenterPos = _vPos; }
    Vec2    GetCenterPos() { return m_vCenterPos; }

    const   tMonInfo& GetInfo() { return m_tMonInfo; }

    int     GetHP() { return m_iHP; }

    void    SetAI(AI* _AI);

public:
    bool    IsCollidingWithStage() const { return !m_CollisionPoint.empty(); }
    void    ReduceHP(int _damage);

private:
    void    SetMonInfo(const tMonInfo& _Info) { m_tMonInfo = _Info; }
    void    DropItem();

    void OnDeath(); // ���Ͱ� �׾��� �� ȣ��Ǵ� �Լ�
    void CheckDeathAnimation();

public:
    virtual void OnCollision(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _ColTag, CCollider* _pOther);

    virtual void OnWallCollision(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnWallCollisionEnter(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnWallCollisionExit(CCollider* _ColTag, CCollider* _pOther);

    virtual void OnStageEndCollision(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnStageEndCollisionEnter(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnStageEndCollisionExit(CCollider* _ColTag, CCollider* _pOther);

public:
    void SetPixelCollider(CColliderPixel* _pPixelCollider) { m_pPixelCollider = _pPixelCollider; }

    vector<Vec2>GetCollisionPoint(const Vec2& _vPos, int _iMonHeightHalf);
    bool CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag);
    void UpdateCollisionState(bool& _bIsColiding, bool _bCollisionDetected, const string& _strColTag, void(CMonster::* onEnter)(), void(CMonster::* onExit)());
    void CheckPixelColor();

    // �浹ó�� �ݹ�
    void OnStageCollisionEnter();
    void OnStageCollisionExit();

    void OnWallCollisionEnter();
    void OnWallCollisionExit();

    void OnStageEndCollisionEnter();
    void OnStageEndCollisionExit();

public:
    void Update_Animation();
    void MonsterAnimationClip();

    void AddAnimationClip(const wstring& strKey, const wchar_t* pFilePath, int iFrameMax, float fAnimationLimitTime, float fFrameSizeX, float fFrameSizeY);


public:
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CMonster);
};