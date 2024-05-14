#include "../pch.h"
#include "CCore.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CPathManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CEventManager.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CUIManager.h"
#include "../Manager/CColliderManager.h"
#include "../Manager/CCamera.h"
#include "../Module/SelectGDI.h"
#include "../Resource/CTexture.h"

CCore::CCore() :
	m_hWnd(0),
	m_ptResolution{},
	m_hDC(0),
	m_arrBrush{},
	m_arrPen{}
{
	CreateHBrush();
	CreateHPen();
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);

	for (UINT i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}
}

int CCore::Init(HWND _hWnd, POINT _ptResolution)
{
	// main.cpp의 48번째 줄에서 값을 받아온다. _hwnd = g_hWnd, _ptResolution = POINT { 1280, 760 }
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// 해상도에 맞게 윈도우 크기 조정
	RECT rt = { 0, 0, _ptResolution.x, _ptResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_hDC = GetDC(m_hWnd);

	// 이중 버퍼링 용도의 택스쳐 한장을 만든다.
	m_pMemTex = CResourceManager::GetInst()->CreateTexture(L"BackBuffer", (UINT)m_ptResolution.x, (UINT)m_ptResolution.y);

	// Manager 초기화
	CPathManager::GetInst()->Init();
	CTimeManager::GetInst()->Init();
	CKeyManager::GetInst()->Init();
	CCamera::GetInst()->Init(RESOLUTION(m_ptResolution.x, m_ptResolution.y), RESOLUTION(1368, 1071));
	CSceneManager::GetInst()->Init();

	return S_OK;
}

void CCore::progress()
{
	// Manager Update
	CTimeManager::GetInst()->Update();
	CKeyManager::GetInst()->Update();
	CCamera::GetInst()->Update();
	CSceneManager::GetInst()->Update();

	CColliderManager::GetInst()->Update();

	CUIManager::GetInst()->Update();

	Clear();

	// Rendering
	CSceneManager::GetInst()->Render(m_pMemTex->GetDC());
	CCamera::GetInst()->Render(m_pMemTex->GetDC());

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,
		m_pMemTex->GetDC(), 0, 0, SRCCOPY);

	CEventManager::GetInst()->Update();
}

void CCore::Clear()
{
	SelectGDI gdi(m_pMemTex->GetDC(), BRUSH_TYPE::BLACK);
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
}

void CCore::CreateHBrush()
{
	// Hollow Brush
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	// Black Brush
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);
}

void CCore::CreateHPen()
{
	// Red Pen
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	// Blue Pen
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

	// Green Pen
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
}