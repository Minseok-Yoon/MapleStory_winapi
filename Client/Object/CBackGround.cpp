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

void CBackGround::Init(SCENE_TYPE eSceneType)
{
    m_pPixelCollider = new CColliderPixel();

    switch (eSceneType)
    {
    case SCENE_TYPE::STAGE_01:
        if (!m_pPixelCollider->SetPixelInfo(L"texture\\MainLoad.bmp"))
        {
            OutputDebugStringA("Failed to set pixel collider for Stage 01.\n");
        }
        break;

    case SCENE_TYPE::STAGE_02:
        if (!m_pPixelCollider->SetPixelInfo(L"texture\\Stage1Load.bmp"))
        {
            OutputDebugStringA("Failed to set pixel collider for Stage 02.\n");
        }
        break;

    default:
        OutputDebugStringA("Unknown scene type.\n");
        break;
    }
}

void CBackGround::Update()
{
}

void CBackGround::Render(HDC _dc)
{
}