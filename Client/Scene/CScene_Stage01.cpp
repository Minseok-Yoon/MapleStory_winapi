#include "../pch.h"
#include "CScene_Stage01.h"
#include "../Layer/CLayer.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"

CScene_Stage01::CScene_Stage01()
{
}

CScene_Stage01::~CScene_Stage01()
{
}

void CScene_Stage01::Update()
{
}

void CScene_Stage01::Render(HDC _dc)
{
	CScene::Render(_dc);

	CScene::Render(_dc);

	CTexture* pTex = CResourceManager::GetInst()->FindTexture(L"InGameBackGround");

	if (nullptr != pTex)
	{
		int iTexWidth = pTex->Width();
		int iTexHeight = pTex->Height();

		BitBlt(_dc, 0, 0, iTexWidth, iTexHeight, pTex->GetDC(), 0, 0, SRCCOPY);
	}
}

void CScene_Stage01::Start()
{
	CLayer* pLayer = CreateLayer(L"InGameBackGround", 0, LAYER_TYPE::BACKGROUND);

	CResourceManager::GetInst()->LoadTexture(L"InGameBackGround", L"texture\\Main.bmp");
}

void CScene_Stage01::Exit()
{
	DeleteAll();
}
