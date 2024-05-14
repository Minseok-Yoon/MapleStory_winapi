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
		ChangeScene(SCENE_TYPE::START);
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
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	BitBlt(_dc, 0, 0, (int)vResolution.x, (int)vResolution.y, m_pBackGround->GetDC(), 0, 0, SRCCOPY);

	CScene::Render(_dc);
}

void CScene_Stage01::Enter()
{
	CObject* pPlayer = new CPlayer;
	pPlayer->SetObjName(L"Player");
	pPlayer->SetPos(Vec2(240.f, 384.f));
	pPlayer->SetScale(Vec2(100.f, 100.f));
	AddObject(pPlayer, OBJECT_TYPE::PLAYER);

	RegisterPlayer(pPlayer);

	CCamera::GetInst()->SetTarget(pPlayer);

	// Camera Look ÁöÁ¤
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	

	CCamera::GetInst()->FadeIn(1.f);

	FastUpdate();
}

void CScene_Stage01::Exit()
{
	DeleteAll();
}