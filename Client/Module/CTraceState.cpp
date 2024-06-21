#include "../pch.h"
#include "CTraceState.h"

#include "../Manager/CSceneManager.h"
#include "../Scene/CScene.h"
#include "../Object/CPlayer.h"
#include "../Object/CMonster.h"
#include "../Manager/CTimeManager.h"

CTraceState::CTraceState() :
	CState(MON_STATE::TRACE)
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::Update()
{
	// 타겟팅 된 Player를 쫒아간다.
	CPlayer* pPlayer = (CPlayer*)CSceneManager::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	Vec2 vMonPos = GetMonster()->GetPos();

	Vec2 vMonDir = vPlayerPos - vMonPos;
	vMonDir.Normalize();

	vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDeltaTime;

	GetMonster()->SetPos(vMonPos);
}

void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}
