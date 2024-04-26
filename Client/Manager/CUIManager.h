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
	// �θ� UI������ ������ Ÿ���� �� UI�� ã�Ƽ� ��ȯ�Ѵ�.
	CUI* GetFocusedUI();
	CUI* GetTargetedUI(CUI* _pParentUI);

public:
	void Update();

};

