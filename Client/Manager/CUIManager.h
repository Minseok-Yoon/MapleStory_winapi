#pragma once

class CUI;

class CUIManager
{
	SINGLETON(CUIManager);

private:
	CUI* m_pFocusedUI;

public:
	void SetFocusedUI(CUI* _pUI);

private:
	// 부모 UI내에서 실제로 타겟팅 된 UI를 찾아서 반환한다.
	CUI* GetFocusedUI();
	CUI* GetTargetedUI(CUI* _pParentUI);

public:
	void Update();

};

