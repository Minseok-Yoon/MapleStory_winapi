#include "../pch.h"
#include "CInventory.h"

SLOT CInventory::m_tSlot[8][16];
bool CInventory::m_bItemMove;

CInventory::CInventory() :
	m_iRow(0),
	m_iCol(0)
{
}

CInventory::~CInventory()
{
}

void CInventory::MouseOn()
{
}

void CInventory::MouseLbtnDown()
{
}

void CInventory::MouseLbtnUp()
{
}

void CInventory::Update()
{
}

void CInventory::Render(HDC _dc)
{
}
