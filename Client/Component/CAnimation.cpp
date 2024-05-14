#include "../pch.h"
#include "CAnimation.h"
#include "../Resource/CTexture.h"

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


void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
}

void CAnimation::Update()
{
}

void CAnimation::Render(HDC _dc)
{
}
