#include "../pch.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "../Object/CObject.h"
#include "../Resource/CTexture.h"
#include "../Manager/CTimeManager.h"

CAnimation::CAnimation()	:
	m_pAnimator(nullptr),
	m_pTex(nullptr),
	m_iCurFrm(0),
	m_fAccTime(0.f),
	m_bFinish(false),
	m_bPause(false)
{
}

CAnimation::~CAnimation()
{
}

// Atlas 애니메이션. 큰 이미지에서 여러 프레임을 잘라내어 사용하는 애니메이션 방식
// _vLT, _vSliceSize, _vStep을 사용하여 프레임의 시작 위치와 크기, 간격 설정
void CAnimation::CreateAtlas(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, 
	Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	// 기존 텍스처들을 저장할 멤버 변수
	m_pTex = _pTex;

	tAnimFrm frm = {};

	// frameCount : 만들어내야할 프레임 갯수
	for (UINT i = 0; i < _iFrameCount; ++i)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * (float)i;

		m_vecFrm.push_back(frm);
	}
}

// Frame 애니메이션. 여러 개의 텍스처를 순차적으로 사용하는 방식.
void CAnimation::CreateFrame(const vector<CTexture*>& _vecTextures, Vec2 _vLT, 
	Vec2 _vSliceSize, float _fDuration)
{
	m_vecFrm.clear();

	tAnimFrm frm = {};

	// 텍스처 벡터의 크기만큼 반복
	for (size_t i = 0; i < _vecTextures.size(); ++i)
	{
		frm.pTexture = _vecTextures[i];
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT;

		m_vecFrm.push_back(frm);
	}
}

void CAnimation::Update()
{
	if (m_bFinish || m_bPause)
		return;

	m_fAccTime += fDeltaTime;

	if (m_fAccTime >= m_vecFrm[m_iCurFrm].fDuration)
	{
		m_fAccTime = 0.f;
		++m_iCurFrm;

		if (m_iCurFrm >= m_vecFrm.size())
		{
			m_iCurFrm = 0;
			m_bFinish = true;
			m_bEnd = true;
		}
	}
}

void CAnimation::Render(HDC _dc)
{
	if (m_bFinish)
		return;

	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();
	vPos += m_vecFrm[m_iCurFrm].vOffSet;	// Object Position에 Offset만큼 추가 이동위치

	// 렌더링 좌표로 변환
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	// 현재 프레임의 텍스처를 가져옵니다.
	CTexture* m_pTex = m_vecFrm[m_iCurFrm].pTexture;

	TransparentBlt(_dc,
		(int)(vPos.x - (m_vecFrm[m_iCurFrm].vSlice.x / 2.f)),
		(int)(vPos.y - (m_vecFrm[m_iCurFrm].vSlice.y / 2.f)),
		(int)(m_vecFrm[m_iCurFrm].vSlice.x),
		(int)(m_vecFrm[m_iCurFrm].vSlice.y),
		m_pTex->GetDC(),
		(int)(m_vecFrm[m_iCurFrm].vLT.x),
		(int)(m_vecFrm[m_iCurFrm].vLT.y),
		(int)(m_vecFrm[m_iCurFrm].vSlice.x),
		(int)(m_vecFrm[m_iCurFrm].vSlice.y),
		RGB(255, 255, 255));
}