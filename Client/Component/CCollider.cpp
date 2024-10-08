#include "../pch.h"
#include "CCollider.h"
#include "../Object/CObject.h"
#include "../Module/SelectGDI.h"

UINT CCollider::g_iNextID = 0;
bool CCollider::g_bRenderColliders = false;

CCollider::CCollider()	:
	m_pOwner(nullptr),
	m_iID(g_iNextID++),
	m_iCol(0),
	m_bEnable(true)
{
}

CCollider::CCollider(const CCollider& _origin)	:
	m_pOwner(nullptr),
	m_vOffsetPos(_origin.m_vOffsetPos),
	m_vScale(_origin.m_vScale),
	m_iID(g_iNextID++),
	m_bEnable(_origin.m_bEnable)
{
}

CCollider::~CCollider()
{
}

void CCollider::OnCollision(CCollider* _ColTag, CCollider* _pOther)
{
	if (m_pOwner)
	{
		m_pOwner->OnCollision(_ColTag, _pOther);
	}
}

void CCollider::OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther)
{
	++m_iCol;
	if (m_pOwner)
	{
		m_pOwner->OnCollisionEnter(_ColTag, _pOther);
	}
}

void CCollider::OnCollisionExit(CCollider* _ColTag, CCollider* _pOther)
{
	--m_iCol;
	if (m_pOwner)
	{
		m_pOwner->OnCollisionExit(_ColTag, _pOther);
	}
}

void CCollider::FinalUpdate()
{
	// 충돌체가 비활성화된 경우, 업데이트 중단
	if (!m_bEnable)
		return;

	// 충돌체의 소유자 오브젝트의 위치를 가져온다.
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;

	assert(0 <= m_iCol);
}

void CCollider::Render(HDC _dc)
{
	// 충돌체가 비활성화된 경우, 또는 렌더링이 비활성화된 경우 렌더링 중단
	if (!m_bEnable || !g_bRenderColliders)
		return;

	PEN_TYPE ePen = PEN_TYPE::GREEN;

	if (m_iCol)
		ePen = PEN_TYPE::RED;

	SelectGDI P(_dc, ePen);
	SelectGDI B(_dc, BRUSH_TYPE::HOLLOW);

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vFinalPos);

	Rectangle(_dc,
		(int)(vRenderPos.x - m_vScale.x / 2.f),
		(int)(vRenderPos.y - m_vScale.y / 2.f),
		(int)(vRenderPos.x + m_vScale.x / 2.f),
		(int)(vRenderPos.y + m_vScale.y / 2.f));
}

// 게임에서 물체간의 충돌 감지를 수행하는 데 사용.