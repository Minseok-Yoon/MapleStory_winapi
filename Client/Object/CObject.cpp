#include "../pch.h"
#include "CObject.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CResourceManager.h"
#include"../Resource/CTexture.h"
#include "../Resource/CResource.h"
#include "../Component/CCollider.h"
#include "../Component/CAnimator.h"
#include "../Component/CGravity.h"

CObject::CObject() :
	m_vPos{},
	m_vScale{},
	m_pCollider(nullptr),
	m_pAnimator(nullptr),
	m_pGravity(nullptr),
	m_bAlive(true)
{
}

CObject::CObject(const CObject& _origin)	:
	m_strObjName(_origin.m_strObjName),
	m_vPos(_origin.m_vPos),
	m_vScale(_origin.m_vScale),
	m_pCollider(nullptr),
	m_pAnimator(nullptr),
	m_pGravity(nullptr),
	m_bAlive(true)
{
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}

	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}

	if (_origin.m_pGravity)
	{
		m_pGravity = new CGravity(*_origin.m_pGravity);
		m_pGravity->m_pOwner = this;
	}
}

CObject::~CObject()
{
	if (nullptr != m_pCollider)
		delete m_pCollider;

	if (nullptr != m_pAnimator)
		delete m_pAnimator;

	if (nullptr != m_pGravity)
		delete m_pGravity;
}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->m_pOwner = this;
}

void CObject::CreateAnimator()
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_pOwner = this;
}

void CObject::CreateGravity()
{
	m_pGravity = new CGravity;
	m_pGravity->m_pOwner = this;
}

void CObject::FinalUpdate()
{
	if (m_pCollider)
		m_pCollider->FinalUpdate();

	if (m_pAnimator)
		m_pAnimator->FinalUpdate();

	if (m_pGravity)
		m_pGravity->FinalUpdate();
}

void CObject::Render(HDC _dc)
{
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPos);

	Rectangle(_dc,
		(int)(vRenderPos.x - m_vScale.x / 2.f),
		(int)(vRenderPos.y - m_vScale.y / 2.f),
		(int)(vRenderPos.x + m_vScale.x / 2.f),
		(int)(vRenderPos.y + m_vScale.y / 2.f));

	ComponentRender(_dc);
}

void CObject::ComponentRender(HDC _dc)
{
	if (nullptr != m_pAnimator)
	{
		m_pAnimator->Render(_dc);
	}


	if (nullptr != m_pCollider)
	{
		m_pCollider->Render(_dc);
	}
}