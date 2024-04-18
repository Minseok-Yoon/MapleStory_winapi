#pragma once

class CTimeManager
{
	SINGLETON(CTimeManager);

private:
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llFrequency;

	double			m_dDeltaTime;	// 프레임 간의 시간값
	double			m_dAcc;			// 1초 체크를 위한 누적 시간
	UINT			m_iCallCount;	// 함수 호출 횟수 체크
	UINT			m_iFPS;			// 초당 호출 횟수

public:
	void Init();
	void Update();
	void Render();

public:
	double GetDeltaTime() { return m_dDeltaTime; }
	float GetfDeltaTime() { return (float)m_dDeltaTime; }
};