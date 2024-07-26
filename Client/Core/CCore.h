#pragma once

class CTexture;

class CCore
{
	SINGLETON(CCore);

private:
	HDC			m_hDC;								// 메인 윈도우에 그릴 DC
	HWND		m_hWnd;								// 메인 윈도우 핸들
	POINT		m_ptResolution;						// 메인 윈도우 해상도
	HBRUSH		m_arrBrush[(UINT)BRUSH_TYPE::END];	// 브러쉬 배열 생성
	HPEN		m_arrPen[(UINT)PEN_TYPE::END];		// 펜 배열 생성
	
	bool		m_bRenderOption;
	CTexture*	m_pMemTex;							// 백버퍼 택스처

public:
	int Init(HWND _hWnd, POINT _ptResolution);		// 게임 엔진 초기화 함수
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