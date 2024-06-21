#include "../pch.h"
#include "CBackGround.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CColliderManager.h"
#include "../Manager/CCamera.h"
#include "../Resource/CResource.h"
#include "../Resource/CTexture.h"
#include "../Core/CCore.h"
#include "../Component/CColliderPixel.h"

CBackGround::CBackGround()	:
	m_pTexture(nullptr),
	m_pPixelCollider(nullptr)
{
}

CBackGround::~CBackGround()
{
	delete m_pPixelCollider;
}

void CBackGround::Init(const wstring& _strKey)
{
	m_pPixelCollider = new CColliderPixel();
	if (!m_pPixelCollider->SetPixelInfo(L"texture\\MainLoad.bmp"))
	{
		OutputDebugStringA("Failed to set pixel collider.\n");
	}
}

void CBackGround::Update()
{
}

void CBackGround::Render(HDC _dc)
{
}