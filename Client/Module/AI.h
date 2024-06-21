#pragma once

class CMonster;
class CState;

class AI
{
	friend class CMonster;

public:
	AI();
	~AI();

private:
	map<MON_STATE, CState*>	m_mapState;			// ������ ���¸� �ʿ� ����
	CState*					m_pCurState;		// ������ ���� ����
	CMonster*				m_pMonster;			// ������ ��ü�� ����

public:
	void SetCurState(MON_STATE _eState);		// AI�� ���� ���¸� ���޵� ���·� ����
	CState* GetCurState(MON_STATE _eState);

	CMonster* GetMonOwner() { return m_pMonster; }

	void AddState(CState* _pState);				// AI�� ���ο� ���¸� �߰�
	void ChangeState(MON_STATE _eNextState);	// Ai�� ���¸� ����

public:
	void Update();
};