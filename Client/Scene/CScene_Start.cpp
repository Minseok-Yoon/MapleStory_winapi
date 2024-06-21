#include "../pch.h"
#include "CScene_Start.h"
#include "../Resource/CTexture.h"
#include "../Manager/CUIManager.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CSceneManager.h"
#include "../Object/CBtnUI.h"
#include "../Manager/CCamera.h"
#include "../Core/CCore.h"
#include "../Object/CUIPanel.h"
#include "../Object/CUI.h"

void ChangeSceneByBtn(DWORD_PTR, DWORD_PTR);

CScene_Start::CScene_Start()	:
	m_pUI(nullptr)
{
	m_pTexture = CResourceManager::GetInst()->LoadTexture(L"StartBackGround", L"texture\\StartPanel.bmp");
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

	CBtnUI* pStartBtn = new CBtnUI;
	pStartBtn->SetObjName(L"StartBtn");
	pStartBtn->SetScale(Vec2(256.f, 100.f));
	pStartBtn->SetPos(Vec2(500.f, 300.f));
	pStartBtn->SetBtnTexture(L"StartBtn", L"texture\\StartBtn.bmp");
	pStartBtn->SetClickedCallBack(ChangeSceneByBtn, 0, 0);
	AddObject(pStartBtn, OBJECT_TYPE::UI);

	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
}

void CScene_Start::Exit()
{
	CCamera::GetInst()->FadeOut(1.f);
	DeleteAll();
}

void CScene_Start::Update()
{
	CScene::Update();

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::STAGE_01);
	}
}

void CScene_Start::Render(HDC _dc)
{
	//CTexture* pTex = CResourceManager::GetInst()->FindTexture(L"StartBackGround");
	
	if (nullptr != m_pTexture)
	{
		int iTexWidth = m_pTexture->Width();
		int iTexHeight = m_pTexture->Height();
	
		BitBlt(_dc, 0, 0, iTexWidth, iTexHeight, m_pTexture->GetDC(), 0, 0, SRCCOPY);
	}

	CScene::Render(_dc);
}

void ChangeSceneByBtn(DWORD_PTR, DWORD_PTR)
{
	ChangeScene(SCENE_TYPE::STAGE_01);
}