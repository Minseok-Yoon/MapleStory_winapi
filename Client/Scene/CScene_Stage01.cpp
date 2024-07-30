#include "../pch.h"
#include "CScene_Stage01.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"
#include "../Manager/CCamera.h"
#include "../Core/CCore.h"
#include "../Manager/CKeyManager.h"
#include "../Scene/CScene.h"
#include "../Object/CPlayer.h"
#include "../Object/CMonster.h"
#include "../Manager/CColliderManager.h"
#include "../Factory/CMonFactory.h"
#include "../Component/CColliderPixel.h"
#include "../Object/CBackGround.h"
#include "../Object/CPortal.h"

CScene_Stage01::CScene_Stage01()
{
	m_pBackGround = CResourceManager::GetInst()->LoadTexture(L"InGameBackGround", L"texture\\Main.bmp");
	SetMapSize(Vec2(1368.f, 1061.f));
}

CScene_Stage01::~CScene_Stage01()
{
}

void CScene_Stage01::Update()
{
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::STAGE_02);
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

void CScene_Stage01::Render(HDC _dc)
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

void CScene_Stage01::Enter()
{
	// 배경 객체 초기화
	CBackGround* pBackGround = new CBackGround();
	pBackGround->Init(L"InGameBackGround");
	AddObject(pBackGround, OBJECT_TYPE::PIXEL_BACKGROUND);

	// 포탈 객체 초기화
	CPortal* pPortal = new CPortal();
	pPortal->SetObjName(L"Portal");
	pPortal->SetPortalTag(L"Portal 0");
	pPortal->SetPos(Vec2(150.f, 820.f));
	AddObject(pPortal, OBJECT_TYPE::PORTAL);

	// 플레이어 객체 초기화
	CPlayer* pPlayer = new CPlayer();
	pPlayer->SetObjName(L"Player");
	pPlayer->SetPos(Vec2(760.f, 777.f));
	pPlayer->SetScale(Vec2(100.f, 100.f));
	pPlayer->SetPixelCollider(pBackGround->GetPixelCollider());
	AddObject(pPlayer, OBJECT_TYPE::PLAYER);

	RegisterPlayer(pPlayer);

	CCamera::GetInst()->SetTarget(pPlayer);

	// 몬스터 배치
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	// 몬스터 생성할 위치 리스트
	std::vector<Vec2> monsterPositions = {
		Vec2(350.f, 415.f),
		Vec2(650.f, 360.f),
		Vec2(150.f, 820.f),
		Vec2(860.f, 540.f)
	};

	// 몬스터 생성 반복문
	for (const auto& pos : monsterPositions) {
		CMonster* pMonster = CMonFactory::CreateMonster(MON_TYPE::NORMAL, pos);
		pMonster->SetObjName(L"Monster");
		pMonster->SetPixelCollider(pBackGround->GetPixelCollider());
		AddObject(pMonster, OBJECT_TYPE::MONSTER);
	}

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

void CScene_Stage01::Exit()
{
	DeleteAll();
}