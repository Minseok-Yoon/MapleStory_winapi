#include "../pch.h"
#include "CTimeManager.h"
#include "../Core/CCore.h"

CTimeManager::CTimeManager() :
	m_llCurCount{},
	m_llPrevCount{},
	m_llFrequency{},
	m_dDeltaTime(0.f),
	m_dAcc(0.),
	m_iCallCount(0)
{
}

CTimeManager::~CTimeManager()
{
}

void CTimeManager::Init()
{
	// 현재 카운트
	QueryPerformanceCounter(&m_llPrevCount);

	// 초당 카운트 횟수
	QueryPerformanceFrequency(&m_llFrequency);
}

void CTimeManager::Update()
{
	QueryPerformanceCounter(&m_llCurCount);

	// 이전 프레임의 카운팅과, 현재 프레임 카운팅 값의 차이를 구한다.
	// 즉 '한 프레임에 걸린 시간값'을 구하고 싶다.
	m_dDeltaTime = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;

	// 이전카운트 값을 현재값으로 갱신(다음번에 계산을 위해서)
	m_llPrevCount = m_llCurCount;

	Render();

	// 최소 프레임이 60미만으로 안떨어지게 설정
#ifdef  _DEBUG
	if (m_dDeltaTime > (1. / 60.))
		m_dDeltaTime = (1. / 60.);
#endif //  _DEBUGG
}

// 프레임 속도(FPS)를 계산하고, 누적된 프레임 시간(DeltaTime)을 갱신하는 역할을 한다.
void CTimeManager::Render()
{
	++m_iCallCount;
	m_dAcc += m_dDeltaTime;	// DT 누적

	if (m_dAcc >= 1.)
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0.f;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d, DeltaTime : %f", m_iFPS, m_dDeltaTime);
		SetWindowText(CCore::GetInst()->GetMainHWnd(), szBuffer);
	}
}

// FPS와 DeltaTime을 화면의 오른쪽 아래에 출력하고 싶은데, 그래픽스 API(OpenGL, DirectX)를 사용하지 않으면
// 사실상 구현하기가 빡셈
// 혹은 게임 엔진(Unity 등)에서 제공을 해준다고 한다.