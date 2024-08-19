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

    // 몬스터의 생명력 체크
    if (pMonster->GetHP() <= 0)
    {
        ChangeAIState(GetAI(), MON_STATE::DEAD);
        return; // 죽은 상태에서 더 이상 추적 로직을 실행하지 않음
    }

    // 몬스터의 충돌 상태를 확인
    bool bIsCollidingWithStage = !pMonster->m_CollisionPoint.empty();
    bool bIsCollidingWithStageEnd = false;

    if (bIsCollidingWithStageEnd)
    {
        // 반대 방향으로 잠깐 이동
        Vec2 vReverseDir = -vMonDir; // 반대 방향으로 설정
        vMonPos += vReverseDir * GetMonster()->GetInfo().fSpeed * fDeltaTime;

        // 잠깐 이동한 후, 다시 추적 상태로 전환
        m_fLostTime = 0.f; // 반대 방향으로 이동한 후 추적 상태로 돌아가기 위한 설정
        ChangeAIState(GetAI(), MON_STATE::PATROL); // 잠깐 이동 후 PATROL 상태로 돌아가게 설정
    }
    else
    {
        // Y축 이동 제어
        if (!bIsCollidingWithStage)
        {
            vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDeltaTime;
        }
        else
        {
            // 충돌 중일 때 Y축 이동을 막고 X축 이동만 수행
            vMonPos.x += vMonDir.x * GetMonster()->GetInfo().fSpeed * fDeltaTime;
        }

        // 추적 범위 체크 (X축만 고려)
        float fXDiff = fabs(vPlayerPos.x - vMonPos.x); // X축 거리 계산
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