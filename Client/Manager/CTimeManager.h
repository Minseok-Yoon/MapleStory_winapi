#pragma once

class CTimeManager
{
	SINGLETON(CTimeManager);

private:
	LARGE_INTEGER	m_llCurCount;	// ���� �ð��� ��Ÿ���� ����
	LARGE_INTEGER	m_llPrevCount;	// ���� �����ӿ����� �ð��� ��Ÿ���� ����
	LARGE_INTEGER	m_llFrequency;	

	double			m_dDeltaTime;	// ������ ���� �ð���
	double			m_dAcc;			// 1�� üũ�� ���� ���� �ð�
	UINT			m_iCallCount;	// �Լ� ȣ�� Ƚ�� üũ
	UINT			m_iFPS;			// �ʴ� ȣ�� Ƚ��

public:
	void Init();
	void Update();
	void Render();

public:
	double GetDeltaTime() { return m_dDeltaTime; }
	float GetfDeltaTime() { return (float)m_dDeltaTime; }
};