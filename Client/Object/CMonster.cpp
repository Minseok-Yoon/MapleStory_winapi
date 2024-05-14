#include "../pch.h"
#include "CMonster.h"
#include "../Component/CCollider.h"
#include "../Module/AI.h"
#include "../Resource/CTexture.h"
#include "../Manager/CResourceManager.h"

CMonster::CMonster()	:
	m_tMonInfo()
{
	m_pTex = CResourceManager::GetInst()->LoadTexture(L"Player", L"texture\\Player\\Player.bmp");

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(40.f, 40.f));
}

CMonster::~CMonster()
{
	if (nullptr != m_pAI)
		delete m_pAI;
}

void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI;
	m_pAI->m_pMonster = this;
}

void CMonster::OnCollision(CCollider* _pOther)
{
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
}

void CMonster::OnCollisionExit(CCollider* _pOther)
{
}

void CMonster::Update()
{
	if (nullptr != m_pAI)
		m_pAI->Update();
}

void CMonster::Render(HDC _dc)
{
	int iWidth = (int)m_pTex->Width();
	int iHeight = (int)m_pTex->Height();

	Vec2 vPos = GetPos();

	TransparentBlt(_dc,
		int(vPos.x - (float)(iWidth / 2)),
		int(vPos.y - (float)(iHeight / 2)),
		iWidth, iHeight,
		m_pTex->GetDC(),
		0, 0, iWidth, iHeight,
		RGB(255, 255, 255));

	CObject::ComponentRender(_dc);
}
