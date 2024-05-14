#pragma once
#include "CObject.h"

class AI;
class CTexture;
class CCollider;

struct tMonInfo
{
    float fSpeed;     // �ӵ�
    float fHP;        // ü��
    float fRecoRange; // ���� ����
    float fAttRange;  // ���� ����
    float fAtt;       // ���ݷ�
};

class CMonster :
    public CObject
{
public:
    CMonster();
    virtual ~CMonster();

private:
    CTexture* m_pTex;
    Vec2        m_vCenterPos;
    float       m_fMaxDistance;
    int         m_iDir;

    tMonInfo    m_tMonInfo;
    AI*         m_pAI;

public:
    void SetSpeed(float _fSpeed) { m_tMonInfo.fSpeed = _fSpeed; }
    float GetSpeed() { return m_tMonInfo.fSpeed; }

    void SetMoveDistance(float _fDist) { m_fMaxDistance = _fDist; }

    void SetCenterPos(Vec2 _vPos) { m_vCenterPos = _vPos; }
    Vec2 GetCenterPos() { return m_vCenterPos; }

    const tMonInfo& GetInfo() { return m_tMonInfo; }

    void SetAI(AI* _AI);

private:
    void SetMonInfo(const tMonInfo& _Info) { m_tMonInfo = _Info; }

public:
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);

public:
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CMonster);
};