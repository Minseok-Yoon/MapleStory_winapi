#pragma once
#include "CObject.h"

class CUI :
    public CObject
{
    // CUIManager 클래스가 CUI클래스의 private에 접근할 수 있도록 해준다.
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

// UI클래스 내부에는 이런 저런 UI들이 있기 때문에.. 이것을 '추상 클래스'로 만들어서 '객체화'할 수 없게 만들 것이다.
// 계층 구조의 장점: 자식 UI는 부모 UI가 반드시 먼저 Update가 되어야 그 데이터를 가져다가 본인의 상태를 결정 할 수 있음.
