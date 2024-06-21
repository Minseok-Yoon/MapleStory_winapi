#include "../pch.h"
#include "CCamera.h"
#include "../Core/CCore.h"
#include "../Object/CObject.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"

CCamera::CCamera() :
	m_pTargetObj(nullptr),
	m_fTime(0.5f),
	m_fSpeed(0.f),
	m_fAccTime(0.5f),
	m_pVeilTex(nullptr),
	m_listCamEffect{}
{
}

CCamera::~CCamera()
{
}


void CCamera::CalDiff()
{
	// ���� �ð� ����
	m_fAccTime += fDeltaTime;

	if (m_fTime <= m_fAccTime)
	{
		m_vCurLookAt = m_vLookAt;
	}
	else
	{
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;

		if (!vLookDir.IsZero())
		{
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_fSpeed * fDeltaTime;
		}
	}

	m_vDiff = m_vCurLookAt;
	m_vPrevLookAt = m_vCurLookAt;
}

void CCamera::Init(const RESOLUTION& _tRS, const RESOLUTION& _tWorldRS)
{
	m_tClientRS = _tRS;			// { 1280, 760 }
	m_tWorldRS = _tWorldRS;		// { 1368, 1061 }

	// Core Ŭ�������� ��ġ ���� �޾ƿ´�.
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	m_pVeilTex = CResourceManager::GetInst()->CreateTexture(L"CameraVeil", (UINT)vResolution.x, (UINT)vResolution.y);

	m_vLookAt = Vec2(0.f, 0.f);
	m_vCurLookAt = m_vLookAt;
}

void CCamera::Update()
{
	if (m_pTargetObj)
	{
		if (m_pTargetObj->IsDead())
		{
			m_pTargetObj = nullptr;
		}
		else
		{
			m_vLookAt = m_pTargetObj->GetPos() - Vec2(m_tClientRS.iW * 0.5f, m_tClientRS.iH * 0.5f);
		}
	}

	if (m_vLookAt.x < 0)
		m_vLookAt.x = 0;
	else if (m_vLookAt.x > m_tWorldRS.iW - m_tClientRS.iW)
		m_vLookAt.x = m_tWorldRS.iW - m_tClientRS.iW;
	
	if (m_vLookAt.y < 0)
		m_vLookAt.y = 0;
	else if (m_vLookAt.y > m_tWorldRS.iH - m_tClientRS.iH)
		m_vLookAt.y = (m_tWorldRS.iH - m_tClientRS.iH) - 10.f;

	// ȭ�� �߾���ǥ�� ī�޶� LookAt��ǥ���� ���̰� ���
	CalDiff();
}

void CCamera::Render(HDC _dc)
{
	if (m_listCamEffect.empty())
		return;

	// �ð� �������� üũ�ؼ�
	tCamEffect& effect = m_listCamEffect.front();
	effect.fCurTime += fDeltaTime;

	float fRatio = 0.f; // ����Ʈ ���� ����
	fRatio = effect.fCurTime / effect.fDuration;

	if (fRatio < 0.f)
		fRatio = 0.f;
	if (fRatio > 1.f)
		fRatio = 1.f;

	int iAlpha = 0;

	if (CAM_EFFECT::FADE_OUT == effect.eEffect)
	{
		iAlpha = (int)(255.f * fRatio);
	}

	else if (CAM_EFFECT::FADE_IN == effect.eEffect)
	{
		iAlpha = (int)(255.f * (1.f - fRatio));
	}

	BLENDFUNCTION bf = {};

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = iAlpha;

	AlphaBlend(_dc, 0, 0,
		(int)m_pVeilTex->Width(),
		(int)m_pVeilTex->Height(),
		m_pVeilTex->GetDC(),
		0, 0,
		(int)m_pVeilTex->Width(),
		(int)m_pVeilTex->Height(), bf);

	// ���� �ð��� ����Ʈ �ִ� ���� �ð��� �Ѿ ���
	if (effect.fDuration <= effect.fCurTime)
	{
		// ȿ�� ����
		m_listCamEffect.pop_front();
	}
}