#include "../pch.h"
#include "CScene_Start.h"
#include "../Layer/CLayer.h"
#include "../Resource/CTexture.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CSceneManager.h"
#include "../Object/CBtnUI.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{
	CResourceManager::GetInst()->LoadTexture(L"StartBackGround", L"texture\\StartPanel.bmp");

	CLayer* uiLayer = new CLayer();
	CObject* pStartBtn = new CBtnUI;
	pStartBtn->SetObjName(L"StartBtn");
	pStartBtn->SetPos(Vec2(540.f, 240.f));
	pStartBtn->SetScale(Vec2(200.f, 100.f));

	uiLayer->AddObject(pStartBtn, OBJECT_TYPE::UI);
	AddLayer(uiLayer, LAYER_TYPE::UI);

	FastUpdate();
}

void CScene_Start::Exit()
{
	DeleteAll();
}

void CScene_Start::Update()
{
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::STAGE_01);
	}
}

void CScene_Start::Render(HDC _dc)
{
	CScene::Render(_dc);

	CTexture* pTex = CResourceManager::GetInst()->FindTexture(L"StartBackGround");

	if (nullptr != pTex)
	{
		int iTexWidth = pTex->Width();
		int iTexHeight = pTex->Height();

		BitBlt(_dc, 0, 0, iTexWidth, iTexHeight, pTex->GetDC(), 0, 0, SRCCOPY);
	}
}