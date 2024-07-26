#include "../pch.h"
#include "CGravity.h"
#include "../Manager/CTimeManager.h"
#include "../Object/CObject.h"
#include "../Component/CRigidBody.h"

CGravity::CGravity() :
	m_pOwner(nullptr),
	m_bGround(false)
{
}

CGravity::~CGravity()
{
}

void CGravity::SetOnGround(bool _bGround)
{
    m_bGround = _bGround;
    if (m_bGround)
    {
        Vec2 vV = m_pOwner->GetRigidBody()->GetVelocity();
        m_pOwner->GetRigidBody()->SetVelocity(Vec2(vV.x, 0.f));
        //m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 0.f));
    }
}

void CGravity::FinalUpdate()
{
    if (m_bGround)
    {
        // 중력이 비활성화된 경우
        m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 0.f));
    }
    else
    {
        // 중력이 활성화된 경우
        m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 300.f)); // 기존의 중력 가속도
    }
}