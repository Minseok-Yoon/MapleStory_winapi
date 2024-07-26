#pragma once

class CTexture;

class CCore
{
	SINGLETON(CCore);

private:
	HDC			m_hDC;								// ���� �����쿡 �׸� DC
	HWND		m_hWnd;								// ���� ������ �ڵ�
	POINT		m_ptResolution;						// ���� ������ �ػ�
	HBRUSH		m_arrBrush[(UINT)BRUSH_TYPE::END];	// �귯�� �迭 ����
	HPEN		m_arrPen[(UINT)PEN_TYPE::END];		// �� �迭 ����
	
	bool		m_bRenderOption;
	CTexture*	m_pMemTex;							// ����� �ý�ó

public:
	int Init(HWND _hWnd, POINT _ptResolution);		// ���� ���� �ʱ�ȭ �Լ�
	void progress();

private:
	void Clear();
	void CreateHBrush();
	void CreateHPen();

public:
	HDC		GetMainDC() { return m_hDC; }
	HWND	GetMainHWnd() { return m_hWnd; }
	POINT	GetResolution() { return m_ptResolution; }
	HBRUSH	GetBrush(BRUSH_TYPE _eBrush) { return m_arrBrush[(UINT)_eBrush]; }
	HPEN	GetPen(PEN_TYPE _ePen) { return m_arrPen[(UINT)_ePen]; }

	bool GetRenderOption() { return m_bRenderOption; }
};