#include "../pch.h"
#include "CRigidBody.h"
#include "../Object/CObject.h"
#include "../Manager/CTimeManager.h"

CRigidBody::CRigidBody() :
	m_pOwner(nullptr),
	m_fMass(1.f),
	m_fFricCoeff(100.f),
	m_vMaxVelocity(Vec2(200.f, 600.f))
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::Move()
{
	// �̵� �ӷ�
	float fSpeed = m_vVelocity.Length();

	if (0.f != fSpeed)
	{
		// �̵� ����
		Vec2 vDir = m_vVelocity;
		vDir.Normalize();

		Vec2 vPos = m_pOwner->GetPos();
		vPos += m_vVelocity * fDeltaTime;

		m_pOwner->SetPos(vPos);
	}
}

void CRigidBody::FinalUpdate()
{
	// ���� ũ��
	float fForce = m_vForce.Length();

	if (0.f != fForce)
	{
		// ���� ����
		m_vForce.Normalize();

		// ���ӵ��� ũ��
		float m_fAccel = fForce / m_fMass;

		// ���ӵ�
		m_vAccel = m_vForce * m_fAccel;	//m_vForce * m_fAccel;

	}

	// �߰� ���ӵ�
	m_vAccel += m_vAccelA;

	// �ӵ�
	m_vVelocity += m_vAccel * fDeltaTime;

	// �����¿� ���� �ݴ���������� ���ӵ�
	if (!m_vVelocity.IsZero())
	{
		Vec2 vFricDir = -m_vVelocity;
		vFricDir.Normalize();

		Vec2 vFriction = vFricDir * m_fFricCoeff * fDeltaTime;
		if (m_vVelocity.Length() <= vFriction.Length())
		{
			// ���� ���ӵ��� ���� �ӵ����� �� ū ���
			m_vVelocity = Vec2(0.f, 0.f);
		}

		else
		{
			m_vVelocity += vFriction;
		}
	}

	// �ӵ� ���� �˻�
	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	{
		m_vVelocity.x = (m_vVelocity.x / abs(m_vVelocity.x)) * abs(m_vMaxVelocity.x);
	}

	if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
	{
		m_vVelocity.y = (m_vVelocity.y / abs(m_vVelocity.y)) * abs(m_vMaxVelocity.y);
	}

	// �ӵ��� ���� �̵�
	Move();

	// �� �ʱ�ȭ
	m_vForce = Vec2(0.f, 0.f);

	// ���ӵ� �ʱ�ȭ
	m_vAccel = Vec2(0.f, 0.f);
	m_vAccelA = Vec2(0.f, 0.f);
}
