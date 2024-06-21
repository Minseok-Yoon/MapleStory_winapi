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

CScene_Stage01::CScene_Stage01()
{
	m_pBackGround = CResourceManager::GetInst()->LoadTexture(L"InGameBackGround", L"texture\\Main.bmp");
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

	// 플레이어 객체 초기화
	CPlayer* pPlayer = new CPlayer();
	pPlayer->SetObjName(L"Player");
	pPlayer->SetPos(Vec2(100.f, 320.f));
	pPlayer->SetScale(Vec2(100.f, 100.f));
	pPlayer->SetPixelCollider(pBackGround->GetPixelCollider());
	AddObject(pPlayer, OBJECT_TYPE::PLAYER);

	RegisterPlayer(pPlayer);

	CCamera::GetInst()->SetTarget(pPlayer);

	// 몬스터 배치
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300.f));
	AddObject(pMon, OBJECT_TYPE::MONSTER);

	CCamera::GetInst()->FadeIn(1.f);
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

	CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::MONSTER);
	CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::PIXEL_BACKGROUND);

	FastUpdate();
}

void CScene_Stage01::Exit()
{
	DeleteAll();
}