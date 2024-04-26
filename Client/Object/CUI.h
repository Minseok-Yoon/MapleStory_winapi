#pragma once
#include "CObject.h"

class CUI :
    public CObject
{
    // CUIManager Ŭ������ CUIŬ������ private�� ������ �� �ֵ��� ���ش�.
    friend class CUIManager;

public:
    CUI();
    CUI(const CUI& _origin);
    virtual ~CUI();

private:
    CUI*            m_pParentUI;
    vector<CUI*>    m_vecChildUI;
    Vec2            m_vFinalPos;

public:
    CUI* GetParent() { return m_pParentUI; }
    const vector<CUI*>& GetChildUI() { return m_vecChildUI; }
    Vec2 GetFinalPos() { return m_vFinalPos; }

    void AddChild(CUI* _pUI) { m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this; }

private:
    void Update_Child();
    void FinalUpdate_Child();
    void Render_Child(HDC _dc);

public:
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render(HDC _dc);

    virtual CUI* Clone() = 0;
};

// UIŬ���� ���ο��� �̷� ���� UI���� �ֱ� ������.. �̰��� '�߻� Ŭ����'�� ���� '��üȭ'�� �� ���� ���� ���̴�.
// ���� ������ ����: �ڽ� UI�� �θ� UI�� �ݵ�� ���� Update�� �Ǿ�� �� �����͸� �����ٰ� ������ ���¸� ���� �� �� ����.
