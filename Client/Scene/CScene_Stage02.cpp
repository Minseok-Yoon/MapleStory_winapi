#include "../pch.h"
#include "CScene_Stage02.h"
#include "../Core/CCore.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CKeyManager.h"
#include "../Component/CCollider.h"
#include "../Resource/CTexture.h"
#include "../Scene/CScene.h"
#include "../Object/CPlayer.h"
#include "../Object/CBackGround.h"
#include "../Object/CPortal.h"
#include "../Manager/CColliderManager.h"

CScene_Stage02::CScene_Stage02()
{
	m_pBackGround = CResourceManager::GetInst()->LoadTexture(L"InGameBackGround2", L"texture\\Stage1.bmp");
	SetMapSize(Vec2(1420.f, 760.f));
}

CScene_Stage02::~CScene_Stage02()
{
}

void CScene_Stage02::Update()
{
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::STAGE_01);
	}

	if (KEY_TAP(KEY::F1))
	{
		CCollider::ToggleRenderColliders();
	}

	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		const vector<CObject*>& vecObj = GetGroupObject((OBJECT_TYPE)i);

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			vecObj[j]->Update();
		}
	}
}

void CScene_Stage02::Render(HDC _dc)
{
	Vec2 cameraPos = CCamera::GetInst()->GetLookAt();

	// 배경의 시작 위치를 계산합니다.
	int startX = static_cast<int>(cameraPos.x);
	int startY = static_cast<int>(cameraPos.y);

	// 배경의 크기를 가져옵니다.
	int bgWidth = m_pBackGround->Width();
	int bgHeight = m_pBackGround->Height();

	// 배경을 렌더링합니다.
	for (int y = 0; y < CCore::GetInst()->GetResolution().y; y += bgHeight)
	{
		for (int x = 0; x < CCore::GetInst()->GetResolution().x; x += bgWidth)
		{
			BitBlt(_dc, x - startX, y - startY, bgWidth, bgHeight, m_pBackGround->GetDC(), 0, 0, SRCCOPY);
		}
	}
	CScene::Render(_dc);
}

void CScene_Stage02::Enter()
{
	// 배경 객체 초기화
	CBackGround* pBackGround = new CBackGround();
	pBackGround->Init(SCENE_TYPE::STAGE_02);
	AddObject(pBackGround, OBJECT_TYPE::PIXEL_BACKGROUND);

	// 포탈 객체 초기화
	CPortal* pPortal = new CPortal("Portal 1");
	pPortal->SetObjName(L"Portal");
	pPortal->SetPos(Vec2(1280.f, 485.f));
	AddObject(pPortal, OBJECT_TYPE::PORTAL);

	// 플레이어 객체 초기화
	CPlayer* pPlayer = new CPlayer();
	pPlayer->SetObjName(L"Player");
	pPlayer->SetPos(Vec2(1280.f, 475.f));
	pPlayer->SetScale(Vec2(100.f, 100.f));
	pPlayer->SetPixelCollider(pBackGround->GetPixelCollider());
	AddObject(pPlayer, OBJECT_TYPE::PLAYER);

	RegisterPlayer(pPlayer);

	CCamera::GetInst()->SetTarget(pPlayer);

	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CCamera::GetInst()->FadeIn(1.f);
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

	CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::MONSTER);
	CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::PORTAL);
	CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::ITEM);
	CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::MESO);
	CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::PIXEL_BACKGROUND);
	CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::MONSTER, OBJECT_TYPE::PIXEL_BACKGROUND);

	FastUpdate();
}

void CScene_Stage02::Exit()
{
	DeleteAll();

	// 충돌 관리 상태 초기화
	CColliderManager::GetInst()->Reset();
}