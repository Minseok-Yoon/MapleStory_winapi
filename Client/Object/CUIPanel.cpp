#include "../pch.h"
#include "CUIPanel.h"
#include "../Manager/CKeyManager.h"

CUIPanel::CUIPanel() :
	CUI(false)
{
}

CUIPanel::~CUIPanel()
{
}

void CUIPanel::MouseOn()
{
	if (IsLbtnDown())
	{
		Vec2 vDiff = MOUSE_POS - m_vDragStart;

		Vec2 vCurPos = GetPos();
		vCurPos += vDiff;
		SetPos(vCurPos);

		m_vDragStart = MOUSE_POS;
	}
}

void CUIPanel::MouseLbtnDown()
{
	m_vDragStart = MOUSE_POS;
}

void CUIPanel::MouseLbtnUp()
{
}

void CUIPanel::Update()
{
}

void CUIPanel::Render(HDC _dc)
{
	CUI::Render(_dc);
}
