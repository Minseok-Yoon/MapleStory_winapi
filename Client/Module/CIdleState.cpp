#include "../pch.h"
#include "CIdleState.h"

#include "../Manager/CSceneManager.h"
#include "../Scene/CScene.h"
#include "../Object/CPlayer.h"
#include "../Object/CMonster.h"

CIdleState::CIdleState() :
	CState(MON_STATE::IDLE)
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::Update()
{
	// Player의 위치 체크
	CPlayer* pPlayer = (CPlayer*)CSceneManager::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	// 몬스터의 범위 안에 들어오면 추적상태로 전환
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();

	Vec2 vDiff = vPlayerPos - vMonPos;
	float fLen = vDiff.Length();

	// 플레이어가 몬스터의 인식범위 안으로 진입
	if (fLen < pMonster->GetInfo().fRecoRange)
	{
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}
}

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}
