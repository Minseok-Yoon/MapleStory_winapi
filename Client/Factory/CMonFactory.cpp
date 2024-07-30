#include "../pch.h"
#include "CMonFactory.h"
#include "../Object/CMonster.h"
#include "../Module/AI.h"

#include "../Module/CIdleState.h"
#include "../Module/CTraceState.h"

CMonster* CMonFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos)
{
	CMonster* pMon = nullptr;

	switch (_eType)
	{
	case MON_TYPE::NORMAL:
	{
		pMon = new CMonster;
		pMon->SetPos(_vPos);
		pMon->CheckPixelColor();

		tMonInfo info = {};
		info.fAtt = 10.f;
		info.fAttRange = 50.f;
		info.fRecoRange = 200.f;
		info.fHP = 100.f;
		info.fSpeed = 150.f;

		pMon->SetMonInfo(info);

		AI* pAI = new AI;
		pAI->AddState(new CIdleState);
		pAI->AddState(new CTraceState);
		pAI->SetCurState(MON_STATE::IDLE);

		pMon->SetAI(pAI);
	}
	break;

	case MON_TYPE::RANGE:
		break;
	}

	assert(pMon);

	return pMon;
}