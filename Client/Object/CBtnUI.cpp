#include "../pch.h"
#include "CBtnUI.h"
#include "../Manager/CSceneManager.h"
#include "../Scene/CScene_Start.h"
#include "../Scene/CScene.h"

CBtnUI::CBtnUI()	:
	CUI(false),
	m_pFunc(nullptr),
	m_pParam1(0),
	m_pParam2(0),
	m_pSceneFunc(nullptr),
	m_pSceneInst(nullptr)
{
}

CBtnUI::~CBtnUI()
{
}

void CBtnUI::MouseOn()
{
}

void CBtnUI::MouseLBtnDown()
{
}

void CBtnUI::MouseLBtnUp()
{
}

void CBtnUI::MouseLBtnClicked()
{
	if (nullptr != m_pFunc)
	{
		m_pFunc(m_pParam1, m_pParam2);
	}

	if (nullptr != m_pSceneInst && nullptr != m_pSceneFunc)
	{
		((*m_pSceneInst).*m_pSceneFunc)();
	}

	if (nullptr != m_pObjectInst && nullptr != m_pObjectFunc)
	{
		((*m_pObjectInst).*m_pObjectFunc)();
	}
}

void CBtnUI::SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc)
{
	// 버튼 클릭 시 호출할 CScene 객체의 포인터와 멤버 함수의 포인터를 설정합니다.
	m_pSceneInst = _pScene;
	m_pSceneFunc = _pSceneFunc;
}

void CBtnUI::SetClickedCallBack(CObject* _pObject, OBJECT_MEMFUNC _pObjectFunc)
{
	m_pObjectInst = _pObject;
	m_pObjectFunc = _pObjectFunc;
}

void CBtnUI::Update()
{
}

void CBtnUI::Render(HDC _dc)
{
	CUI::Render(_dc);
}