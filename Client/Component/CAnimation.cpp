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

// Atlas �ִϸ��̼�. ū �̹������� ���� �������� �߶󳻾� ����ϴ� �ִϸ��̼� ���
// _vLT, _vSliceSize, _vStep�� ����Ͽ� �������� ���� ��ġ�� ũ��, ���� ����
void CAnimation::CreateAtlas(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, 
	Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	// ���� �ؽ�ó���� ������ ��� ����
	m_pTex = _pTex;

	tAnimFrm frm = {};

	// frameCount : �������� ������ ����
	for (UINT i = 0; i < _iFrameCount; ++i)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * (float)i;

		m_vecFrm.push_back(frm);
	}
}

// Frame �ִϸ��̼�. ���� ���� �ؽ�ó�� ���������� ����ϴ� ���.
void CAnimation::CreateFrame(const vector<CTexture*>& _vecTextures, Vec2 _vLT, 
	Vec2 _vSliceSize, float _fDuration)
{
	m_vecFrm.clear();

	tAnimFrm frm = {};

	// �ؽ�ó ������ ũ�⸸ŭ �ݺ�
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
	vPos += m_vecFrm[m_iCurFrm].vOffSet;	// Object Position�� Offset��ŭ �߰� �̵���ġ

	// ������ ��ǥ�� ��ȯ
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	// ���� �������� �ؽ�ó�� �����ɴϴ�.
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