#pragma once

class CTexture;

class CCore
{
	SINGLETON(CCore);

private:
	HWND		m_hWnd;
	POINT		m_ptResolution;
	HDC			m_hDC;

	CTexture*	m_pMemTex;

public:
	int Init(HWND _hWnd, POINT _ptResolution);
	void progress();

public:
	HWND	GetMainHWnd() { return m_hWnd; }
	HDC		GetMainDC() { return m_hDC; }
	POINT	GetResolution() { return m_ptResolution; }
};