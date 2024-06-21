#include "../pch.h"
#include "CState.h"
#include "AI.h"
#include "../Object/CObject.h"
#include "../Object/CMonster.h"

CState::CState(MON_STATE _eState)	:
	m_eMonState(_eState)
{
}

CState::~CState()
{
}

CMonster* CState::GetMonster()
{
	return m_pAI->GetMonOwner();
}