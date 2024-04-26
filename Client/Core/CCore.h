#pragma once

class CTexture;

class CCore
{
	SINGLETON(CCore);

private:
	HDC			m_hDC;				// 메인 윈도우에 그릴 DC
	HWND		m_hWnd;				// 메인 윈도우 핸들
	POINT		m_ptResolution;		// 메인 윈도우 해상도

	CTexture*	m_pMemTex;			// 백버퍼 택스처

public:
	int Init(HWND _hWnd, POINT _ptResolution);
	void progress();

public:
	HDC		GetMainDC() { return m_hDC; }
	HWND	GetMainHWnd() { return m_hWnd; }
	POINT	GetResolution() { return m_ptResolution; }
};