#include "../pch.h"
#include "CUI.h"

CUI::CUI() :
	m_pParentUI(nullptr),
	m_vecChildUI{},
	m_vFinalPos{}
{
}

CUI::CUI(const CUI& _origin)	:
	CObject(_origin),
	m_pParentUI(nullptr)
{
}

CUI::~CUI()
{
	Safe_Delete_Vec(m_vecChildUI);
}

void CUI::Update_Child()
{
	// 각 요소인 CUI포인터를 사용하여, Object클래스의 Update()함수를 호출한다.
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Update();
	}
}

void CUI::FinalUpdate_Child()
{
	// 각 요소인 CUI포인터를 사용하여, Object클래스의 FinalUpdate()함수를 호출한다.
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->FinalUpdate();
	}
}

void CUI::Render_Child(HDC _dc)
{
	// 각 요소인 CUI포인터를 사용하여, Object클래스의 Render()함수를 호출한다.
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Render(_dc);
	}
}

void CUI::Update()
{
	Update_Child();
}

void CUI::FinalUpdate()
{
	CObject::FinalUpdate();

	FinalUpdate_Child();
}

void CUI::Render(HDC _dc)
{
	Render_Child(_dc);
}
