#include "../pch.h"
#include "CTraceState.h"
#include "../Manager/CSceneManager.h"
#include "../Scene/CScene.h"
#include "../Object/CPlayer.h"
#include "../Object/CMonster.h"
#include "../Manager/CTimeManager.h"

CTraceState::CTraceState() :
	CState(MON_STATE::TRACE),
	m_fLostTime(0.f),
	m_fMaxLostTime(5.f)
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::Update()
{
    CPlayer* pPlayer = (CPlayer*)CSceneManager::GetInst()->GetCurScene()->GetPlayer();
    Vec2 vPlayerPos = pPlayer->GetPos();

    CMonster* pMonster = GetMonster();
    Vec2 vMonPos = pMonster->GetPos();

    Vec2 vMonDir = vPlayerPos - vMonPos;
    vMonDir.Normalize();

    // ������ �浹 ���¸� Ȯ��
    bool bIsCollidingWithStage = !pMonster->m_CollisionPoint.empty();

    // Y�� �̵� ����
    if (!bIsCollidingWithStage)
    {
        vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDeltaTime;
    }
    else
    {
        // �浹 ���� �� Y�� �̵��� ���� X�� �̵��� ����
        vMonPos.x += vMonDir.x * GetMonster()->GetInfo().fSpeed * fDeltaTime;
    }

    GetMonster()->SetPos(vMonPos);

    // ���� ���� üũ
    Vec2 vDiff = vPlayerPos - vMonPos;
    float fLen = vDiff.Length();
    if (fLen >= pMonster->GetInfo().fRecoRange)
    {
        m_fLostTime += fDeltaTime;
        if (m_fLostTime >= m_fMaxLostTime)
        {
            ChangeAIState(GetAI(), MON_STATE::IDLE);
        }
    }
    else
    {
        m_fLostTime = 0.f;
    }
}

void CTraceState::Enter()
{
	m_fLostTime = 0.f;
}

void CTraceState::Exit()
{
}