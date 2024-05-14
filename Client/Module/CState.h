#pragma once

class AI;
class CMonster;

class CState
{
	friend class AI;

public:
	CState(MON_STATE _eState);
	virtual ~CState();

private:
	AI*			m_pAI;
	MON_STATE	m_eMonState;

public:
	AI* GetAI() { return m_pAI; }
	MON_STATE GetStateType() { return m_eMonState; }
	CMonster* GetMonster();

public:
	virtual void Update() = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;
};