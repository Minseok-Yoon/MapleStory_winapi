#include "../pch.h"
#include "CScene_Stage02.h"
#include "../Object/CObject.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CColliderManager.h"
#include "../Resource/CResource.h"
#include "../Resource/CTexture.h"
#include "../Core/CCore.h"
#include "../Object/CPlayer.h"
#include "../Object/CBackGround.h"
#include "../Component/CColliderPixel.h"

CScene_Stage02::CScene_Stage02()	:
	m_pBackGround(nullptr)
{
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
	CScene::Render(_dc);
}

void CScene_Stage02::Enter()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(Vec2(0.f, 0.f));

	CPlayer* pPlayer = new CPlayer();
	pPlayer->SetObjName(L"Player");
	pPlayer->SetPos(Vec2(0.f, 0.f));
	pPlayer->SetScale(Vec2(100.f, 100.f));
	AddObject(pPlayer, OBJECT_TYPE::PLAYER);

	RegisterPlayer(pPlayer);

	CCamera::GetInst()->SetTarget(pPlayer);

	m_pBackGround = new CBackGround();
	m_pBackGround->Init(L"texture\\MainLoad.bmp");
	AddObject(m_pBackGround, OBJECT_TYPE::BACKGROUND);

	//CColliderPixel* pPixel = new CColliderPixel();
	//if (pPixel->SetPixelInfo(L"texture\\MainLoad.bmp"))
	//{
	//	pPlayer->SetPixelCollider(pPixel);
	//	OutputDebugStringA("Pixel collider set successfully.\n");
	//}
	//else
	//{
	//	OutputDebugStringA("Failed to set pixel collider.\n");
	//}

	FastUpdate();
}

void CScene_Stage02::Exit()
{
	DeleteAll();
}