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
    }
}

void CGravity::Jump(float _fJumpForce)
{
    if (!m_bGround) return; // ���� �پ� ���� ������ ���� �Ұ�

    m_bGround = false;
    if (m_pOwner && m_pOwner->GetRigidBody())
    {
        m_pOwner->GetRigidBody()->SetVelocity(Vec2(m_pOwner->GetRigidBody()->GetVelocity().x, -_fJumpForce));
    }
}

void CGravity::FinalUpdate()
{
    if (m_bGround)
    {
        // �߷��� ��Ȱ��ȭ�� ���
        m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 0.f));
    }
    else
    {
        // �߷��� Ȱ��ȭ�� ���
        m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 300.f)); // ������ �߷� ���ӵ�
    }
}