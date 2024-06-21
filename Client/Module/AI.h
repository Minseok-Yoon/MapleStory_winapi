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
	map<MON_STATE, CState*>	m_mapState;			// 몬스터의 상태를 맵에 저장
	CState*					m_pCurState;		// 몬스터의 현재 상태
	CMonster*				m_pMonster;			// 몬스터의 객체를 얻어옴

public:
	void SetCurState(MON_STATE _eState);		// AI의 현재 상태를 전달된 상태로 설정
	CState* GetCurState(MON_STATE _eState);

	CMonster* GetMonOwner() { return m_pMonster; }

	void AddState(CState* _pState);				// AI에 새로운 상태를 추가
	void ChangeState(MON_STATE _eNextState);	// Ai의 상태를 변경

public:
	void Update();
};