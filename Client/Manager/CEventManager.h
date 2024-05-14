#pragma once

struct tEvent
{
	EVENT_TYPE		eEven;		// 이벤트 타입
	DWORD_PTR		lParam;		// lParam 매개변수
	DWORD_PTR		wParam;		// wParam 매개변수
};

class CEventManager
{
	SINGLETON(CEventManager);

private:
	vector<tEvent>		m_vecEvent;		// 이벤트를 저장하는 벡터.
	vector<CObject*>	m_vecDead;		// 삭제된 객체를 저장하는 벡터.

public:
	void Update();

private:
	void Excute(const tEvent& _eve);

public:
	void AddEvent(const tEvent& _eve) { m_vecEvent.push_back(_eve); }
};