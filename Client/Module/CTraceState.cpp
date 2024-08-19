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
	m_fMaxLostTime(5.f),
    m_bReverseMove(false),
    m_fReverseMoveTime(0.f),
    m_fReverseMoveDuration(1.f)
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

    // ������ ����� üũ
    if (pMonster->GetHP() <= 0)
    {
        ChangeAIState(GetAI(), MON_STATE::DEAD);
        return; // ���� ���¿��� �� �̻� ���� ������ �������� ����
    }

    // ������ �浹 ���¸� Ȯ��
    bool bIsCollidingWithStage = !pMonster->m_CollisionPoint.empty();
    bool bIsCollidingWithStageEnd = false;

    if (bIsCollidingWithStageEnd)
    {
        // �ݴ� �������� ��� �̵�
        Vec2 vReverseDir = -vMonDir; // �ݴ� �������� ����
        vMonPos += vReverseDir * GetMonster()->GetInfo().fSpeed * fDeltaTime;

        // ��� �̵��� ��, �ٽ� ���� ���·� ��ȯ
        m_fLostTime = 0.f; // �ݴ� �������� �̵��� �� ���� ���·� ���ư��� ���� ����
        ChangeAIState(GetAI(), MON_STATE::PATROL); // ��� �̵� �� PATROL ���·� ���ư��� ����
    }
    else
    {
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

        // ���� ���� üũ (X�ุ ���)
        float fXDiff = fabs(vPlayerPos.x - vMonPos.x); // X�� �Ÿ� ���
        if (fXDiff >= pMonster->GetInfo().fRecoRange)
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

    GetMonster()->SetPos(vMonPos);
}

void CTraceState::Enter()
{
	m_fLostTime = 0.f;
}

void CTraceState::Exit()
{
}