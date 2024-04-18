#include "../pch.h"
#include "CCore.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CPathManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CKeyManager.h"
#include "../Resource/CResource.h"
#include "../Resource/CTexture.h"

CCore::CCore() :
	m_hWnd(0),
	m_ptResolution{},
	m_hDC(0)
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);
}

int CCore::Init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// �ػ󵵿� �°� ������ ũ�� ����
	RECT rt = { 0, 0, _ptResolution.x, _ptResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_hDC = GetDC(m_hWnd);

	// ���� ���۸� �뵵�� �ý��� ������ �����.
	m_pMemTex = CResourceManager::GetInst()->CreateTexture(L"BackBuffer", (UINT)m_ptResolution.x, (UINT)m_ptResolution.y);

	// Manager �ʱ�ȭ
	CPathManager::GetInst()->Init();
	CTimeManager::GetInst()->Init();
	CKeyManager::GetInst()->init();
	CSceneManager::GetInst()->Init();

	return S_OK;
}

void CCore::progress()
{
	// Manager Update
	CTimeManager::GetInst()->Update();
	CKeyManager::GetInst()->Update();
	CSceneManager::GetInst()->Update();

	// Rendering
	CSceneManager::GetInst()->Render(m_pMemTex->GetDC());

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,
		m_pMemTex->GetDC(), 0, 0, SRCCOPY);

	//CTimeManager::GetInst()->Render();
}