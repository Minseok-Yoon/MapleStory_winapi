#include "../pch.h"
#include "CObject.h"

CObject::CObject() :
	m_vPos{},
	m_vScale{},
	m_bAlive(true)
{
}

CObject::CObject(const CObject& _origin)	:
	m_strObjName(_origin.m_strObjName),
	m_vPos(_origin.m_vPos),
	m_vScale(_origin.m_vScale),
	m_bAlive(true)
{
}

CObject::~CObject()
{
}


void CObject::FinalUpdate()
{
}

void CObject::Render(HDC _dc)
{
}