#include "../pch.h"
#include "CObject.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CResourceManager.h"
#include"../Resource/CTexture.h"
#include "../Resource/CResource.h"
#include "../Component/CCollider.h"
#include "../Component/CColliderPixel.h"
#include "../Component/CAnimator.h"
#include "../Component/CGravity.h"

CObject::CObject() :
	m_vPos{},
	m_vScale{},
	m_vecCollider{},
	m_pAnimator(nullptr),
	m_pGravity(nullptr),
	m_bAlive(true)
{
}

CObject::CObject(const CObject& _origin)	:
	m_strObjName(_origin.m_strObjName),
	m_vPos(_origin.m_vPos),
	m_vScale(_origin.m_vScale),
	m_vecCollider{},
	m_pAnimator(nullptr),
	m_pGravity(nullptr),
	m_bAlive(true)
{
	for (const auto& collider : _origin.m_vecCollider)
	{
		CCollider* newCollider = new CCollider(*collider);
		newCollider->SetOwner(this);
		m_vecCollider.push_back(newCollider);
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
	for (auto collider : m_vecCollider)
	{
		delete collider;
	}

	if (nullptr != m_pAnimator)
		delete m_pAnimator;

	if (nullptr != m_pGravity)
		delete m_pGravity;
}

void CObject::AddCollider()
{
	CCollider* newCollider = new CCollider();
	newCollider->SetOwner(this);
	m_vecCollider.push_back(newCollider);
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
	for (auto collider : m_vecCollider)
	{
		collider->FinalUpdate();
	}

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

	for (auto collider : m_vecCollider)
	{
		collider->Render(_dc);
	}
}