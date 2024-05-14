#include "../pch.h"
#include "CPlayer.h"
#include "../Object/CObject.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CSceneManager.h"
#include "../Scene/CScene.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"
#include "../Component/CCollider.h"

CPlayer::CPlayer()	:
	m_eCurState(PLAYER_STATE::IDLE),
	m_ePrevState(PLAYER_STATE::IDLE),
	m_iDir(1),
	m_iPrevDir(1)
{
	m_pTex = CResourceManager::GetInst()->LoadTexture(L"Player", L"texture\\Player\\Player.bmp");

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2{ 0.f, 0.f });
	GetCollider()->SetScale(Vec2{ 40.f, 40.f });
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update_State()
{
}

void CPlayer::Update_Move()
{
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
}

void CPlayer::Update()
{
	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::W))
	{
		vPos.y -= 500.f * fDeltaTime;
	}

	if (KEY_HOLD(KEY::S))
	{
		vPos.y += 500.f * fDeltaTime;
	}

	if (KEY_HOLD(KEY::A))
	{
		vPos.x -= 500.f * fDeltaTime;
	}

	if (KEY_HOLD(KEY::D))
	{
		vPos.x += 500.f * fDeltaTime;
	}

	SetPos(vPos);

	if (KEY_HOLD(KEY::R))
	{
		SetPos(Vec2(1040.f, 500.f));
	}

	Update_State();
	Update_Move();

	m_ePrevState = m_eCurState;
	m_iPrevDir = m_iDir;
}

void CPlayer::Render(HDC _dc)
{
	int iWidth = (int)m_pTex->Width();
	int iHeight = (int)m_pTex->Height();

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	TransparentBlt(_dc,
		int(vPos.x - (float)(iWidth / 2)),
		int(vPos.y - (float)(iHeight / 2)), 
		iWidth, iHeight,
		m_pTex->GetDC(),
		0, 0, iWidth, iHeight,
		RGB(255, 255, 255));

	CObject::ComponentRender(_dc);
}