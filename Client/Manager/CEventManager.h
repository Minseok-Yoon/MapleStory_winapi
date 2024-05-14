#pragma once

struct tEvent
{
	EVENT_TYPE		eEven;		// �̺�Ʈ Ÿ��
	DWORD_PTR		lParam;		// lParam �Ű�����
	DWORD_PTR		wParam;		// wParam �Ű�����
};

class CEventManager
{
	SINGLETON(CEventManager);

private:
	vector<tEvent>		m_vecEvent;		// �̺�Ʈ�� �����ϴ� ����.
	vector<CObject*>	m_vecDead;		// ������ ��ü�� �����ϴ� ����.

public:
	void Update();

private:
	void Excute(const tEvent& _eve);

public:
	void AddEvent(const tEvent& _eve) { m_vecEvent.push_back(_eve); }
};