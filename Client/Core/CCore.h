#pragma once

class CTexture;

class CCore
{
	SINGLETON(CCore);

private:
	HDC			m_hDC;				// ���� �����쿡 �׸� DC
	HWND		m_hWnd;				// ���� ������ �ڵ�
	POINT		m_ptResolution;		// ���� ������ �ػ�

	CTexture*	m_pMemTex;			// ����� �ý�ó

public:
	int Init(HWND _hWnd, POINT _ptResolution);
	void progress();

public:
	HDC		GetMainDC() { return m_hDC; }
	HWND	GetMainHWnd() { return m_hWnd; }
	POINT	GetResolution() { return m_ptResolution; }
};