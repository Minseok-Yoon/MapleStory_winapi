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
	// ���� ���¸� ���� ���� ���·� ����
	m_pCurState = GetCurState(_eState);

	// ���� ���°� ������ �ߴ�
	if(m_pCurState == nullptr)
		assert(m_pCurState);
}

CState* AI::GetCurState(MON_STATE _eState)
{
	// �ʿ��� ������ �ش� ���¸� ã�´�.
	map<MON_STATE, CState*>::iterator iter = m_mapState.find(_eState);

	// iter�� ����Ű�°� ���� ���κ��̸� nullptr�� ��ȯ
	if (iter == m_mapState.end())
		return nullptr;

	// ã�� ���� ��ü�� �����͸� ��ȯ
	return iter->second;
}

void AI::AddState(CState* _pState)
{
	// ���°� �߰��Ǿ� �ִ��� Ȯ���Ѵ�.
	CState* pState = GetCurState(_pState->GetStateType());

	// �̹� �߰��� ������ ���, �ܾ� ���� �߻�
	if (pState != nullptr)
		assert(nullptr);

	// �ʿ� ���¸� �߰��Ѵ�.
	m_mapState.insert(make_pair(_pState->GetStateType(), _pState));
	_pState->m_pAI = this;
}

void AI::ChangeState(MON_STATE _eNextState)
{
	// ���� ���¸� �����´�.
	CState* pNextState = GetCurState(_eNextState);
	// ���� ���¿� ���� ���°� �������� Ȯ��
	assert(m_pCurState != pNextState);

	m_pCurState->Exit();
	m_pCurState = pNextState;
	m_pCurState->Enter();
}

void AI::Update()
{
	m_pCurState->Update();
}