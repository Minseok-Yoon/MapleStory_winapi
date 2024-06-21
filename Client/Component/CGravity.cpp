#include "../pch.h"
#include "CGravity.h"
#include "../Manager/CTimeManager.h"
#include "../Object/CObject.h"
#include "../Manager/CTimeManager.h"

bool CGravity::m_onLand = false;

CGravity::CGravity()	:
	m_pOwner(nullptr),
	m_bGround(false),
	m_bFalling(true),
	m_fGravityTime(0.f)
{
	m_iFallingCount = 0;
}

CGravity::~CGravity()
{
}

void CGravity::SetOnGround(bool _bGround)
{
	m_bGround = _bGround;
	if (m_bGround)
	{
		m_bFalling = false;
		m_fGravityTime = 0.f;
		m_iFallingCount = 0;
	}
	else
	{
		m_bFalling = true;
	}
	
}

void CGravity::FinalUpdate()
{
	if (!m_pOwner)
		return;

	switch (m_bFalling)
	{
	case true:
		m_fGravityTime += fDeltaTime;

		if (m_fGravityTime >= 0.009f)
		{
			++m_iFallingCount;

			m_fGravityTime -= 0.009f;

			Vec2 vPos = m_pOwner->GetPos();
			vPos += 0.98f * fDeltaTime;

			if (m_iFallingCount <= 10)
				vPos.y += 2.2f;
			else if (m_iFallingCount <= 20)
				vPos.y += 2.5f;
			else
				vPos.y += 3.f;

			m_pOwner->SetPos(vPos);
		}
		break;
	case false:
		break;
	}
}