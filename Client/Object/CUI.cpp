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
	// �� ����� CUI�����͸� ����Ͽ�, ObjectŬ������ Update()�Լ��� ȣ���Ѵ�.
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Update();
	}
}

void CUI::FinalUpdate_Child()
{
	// �� ����� CUI�����͸� ����Ͽ�, ObjectŬ������ FinalUpdate()�Լ��� ȣ���Ѵ�.
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->FinalUpdate();
	}
}

void CUI::Render_Child(HDC _dc)
{
	// �� ����� CUI�����͸� ����Ͽ�, ObjectŬ������ Render()�Լ��� ȣ���Ѵ�.
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
