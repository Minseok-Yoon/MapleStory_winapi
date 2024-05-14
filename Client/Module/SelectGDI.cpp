#include "../pch.h"
#include "SelectGDI.h"
#include "../Core/CCore.h"

SelectGDI::SelectGDI(HDC _dc, PEN_TYPE _ePenType)	:
	m_hDC(_dc),
	m_hDefaultBrush(nullptr),
	m_hDefaultPen(nullptr)
{
	HPEN hPen = CCore::GetInst()->GetPen(_ePenType);
	m_hDefaultPen = (HPEN)SelectObject(_dc, hPen);
}

SelectGDI::SelectGDI(HDC _dc, BRUSH_TYPE _eBrushType) :
	m_hDC(_dc),
	m_hDefaultBrush(nullptr),
	m_hDefaultPen(nullptr)
{
	HBRUSH hBrush = CCore::GetInst()->GetBrush(_eBrushType);
	m_hDefaultBrush = (HBRUSH)SelectObject(_dc, hBrush);
}

SelectGDI::~SelectGDI()
{
	// Device Context에서 사용된 펜과 브러시를 선택 해제하여 이전 상태로 되돌린다.
	SelectObject(m_hDC, m_hDefaultPen);
	SelectObject(m_hDC, m_hDefaultBrush);
}