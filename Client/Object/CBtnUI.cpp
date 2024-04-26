#include "../pch.h"
#include "CBtnUI.h"
#include "../Scene/CScene.h"

CBtnUI::CBtnUI()	:
	CUI(),
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

void CBtnUI::MouseLbtnDown()
{
}

void CBtnUI::MouseLbtnUp()
{
}

void CBtnUI::MouseLbtnClicked()
{
	if (nullptr != m_pFunc)
	{
		m_pFunc(m_pParam1, m_pParam2);
	}

	if (m_pSceneInst && m_pSceneFunc)
	{
		((*m_pSceneInst).*m_pSceneFunc)();
	}
}

void CBtnUI::SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc)
{
	// ��ư Ŭ�� �� ȣ���� CScene ��ü�� �����Ϳ� ��� �Լ��� �����͸� �����մϴ�.
	m_pSceneInst = _pScene;
	m_pSceneFunc = _pSceneFunc;
}
