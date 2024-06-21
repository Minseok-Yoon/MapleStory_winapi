#include "../pch.h"
#include "AI.h"
#include "CState.h"
#include "../Object/CMonster.h"

AI::AI()	:
	m_pCurState(nullptr),
	m_pMonster(nullptr)
{
}

AI::~AI()
{
	Safe_Delete_Map(m_mapState);
}

void AI::SetCurState(MON_STATE _eState)
{
	// 현재 상태를 전달 받은 상태로 설정
	m_pCurState = GetCurState(_eState);

	// 현재 상태가 없으면 중단
	if(m_pCurState == nullptr)
		assert(m_pCurState);
}

CState* AI::GetCurState(MON_STATE _eState)
{
	// 맵에서 몬스터의 해당 상태를 찾는다.
	map<MON_STATE, CState*>::iterator iter = m_mapState.find(_eState);

	// iter가 가리키는게 맵의 끝부분이면 nullptr을 반환
	if (iter == m_mapState.end())
		return nullptr;

	// 찾은 상태 객체의 포인터를 반환
	return iter->second;
}

void AI::AddState(CState* _pState)
{
	// 상태가 추가되어 있는지 확인한다.
	CState* pState = GetCurState(_pState->GetStateType());

	// 이미 추가된 상태인 경우, 단언 실패 발생
	if (pState != nullptr)
		assert(nullptr);

	// 맵에 상태를 추가한다.
	m_mapState.insert(make_pair(_pState->GetStateType(), _pState));
	_pState->m_pAI = this;
}

void AI::ChangeState(MON_STATE _eNextState)
{
	// 다음 상태를 가져온다.
	CState* pNextState = GetCurState(_eNextState);
	// 현재 상태와 다음 상태가 동일한지 확인
	assert(m_pCurState != pNextState);

	m_pCurState->Exit();
	m_pCurState = pNextState;
	m_pCurState->Enter();
}

void AI::Update()
{
	m_pCurState->Update();
}