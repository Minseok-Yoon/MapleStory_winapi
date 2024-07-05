#pragma once
#include "CObject.h"

class AI;
class CTexture;
class CColliderPixel;

struct tMonInfo
{
    string  strTag;    // 이름
    float   fSpeed;     // 속도
    float   fHP;        // 체력
    float   fRecoRange; // 인지 범위
    float   fAttRange;  // 공격 범위
    float   fAtt;       // 공격력
};

class CMonster :
    public CObject
{
    friend class CMonFactory;

public:
    CMonster();
    virtual ~CMonster();

private:
    MON_STATE   m_eCurMonState;
    MON_STATE   m_ePrevMonState;
    CTexture*   m_pTex;
    Vec2        m_vCenterPos;
    float       m_fSpeed;
    float       m_fMaxDistance;
    int         m_iDir;
    int         m_iPrevDir;

    int         m_iHP = 2;
    
    CColliderPixel* m_pPixelCollider;
    tMonInfo    m_tMonInfo;
    AI*         m_pAI;


private:
    float m_fIdleTime; // 대기 시간을 관리하는 변수
    bool m_bWaiting; // 대기 중인지 여부를 나타내는 변수

public:
    void    SetMonTag(string _strTag) { m_tMonInfo.strTag = _strTag; }
    string  GetMonTag() { return m_tMonInfo.strTag; }

    void    SetSpeed(float _fSpeed) { m_tMonInfo.fSpeed = _fSpeed; }
    float   GetSpeed() { return m_tMonInfo.fSpeed; }

    void    SetMoveDistance(float _fDist) { m_fMaxDistance = _fDist; }

    void    SetCenterPos(Vec2 _vPos) { m_vCenterPos = _vPos; }
    Vec2    GetCenterPos() { return m_vCenterPos; }

    const   tMonInfo& GetInfo() { return m_tMonInfo; }

    void    SetAI(AI* _AI);

private:
    void    SetMonInfo(const tMonInfo& _Info) { m_tMonInfo = _Info; }
    void    DropItem();

public:
    virtual void OnCollision(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _ColTag, CCollider* _pOther);

public:
    void SetPixelCollider(CColliderPixel* _pPixelCollider) { m_pPixelCollider = _pPixelCollider; }
    void CheckPixelColor();

public:
    void Update_Animation();
    void MonsterAnimationClip();

    void AddAnimationClip(const wstring& strKey, const wchar_t* pFilePath, int iFrameMax, float fAnimationLimitTime, float fFrameSizeX, float fFrameSizeY);


public:
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CMonster);
};