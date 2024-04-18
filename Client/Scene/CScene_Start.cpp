#include "../pch.h"
#include "CScene_Start.h"
#include "../Layer/CLayer.h"
#include "../Resource/CTexture.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CSceneManager.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{
	CLayer* pLayer = CreateLayer(L"StartBackGround", 0, LAYER_TYPE::BACKGROUND);

	CResourceManager::GetInst()->LoadTexture(L"StartBackGround", L"texture\\StartPanel.bmp");
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