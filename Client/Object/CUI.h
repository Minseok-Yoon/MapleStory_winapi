#pragma once
#include "CObject.h"

class CTexture;

class CUI :
    public CObject
{
    // CUIManager 클래스가 CUI클래스의 private에 접근할 수 있도록 해준다.
    friend class CUIManager;
    friend class CBtnUI;

public:
    CUI(bool _bCamAff);
    CUI(const CUI& _origin);
    virtual ~CUI();

private:
    vector<CUI*>    m_vecChildUI;
    CUI*            m_pParentUI;
    Vec2            m_vFinalPos;

    bool            m_bCamAffected; // UI가 카메라에 영향을 받는 유무
    bool            m_bMouseOn;     // UI 위에 마우스가 있는지
    bool            m_bLbtnDown;    // UI에 왼쪽버튼이 눌린적이 있는지

    CTexture*       m_pTexture;

public:
    CUI* GetParent() { return m_pParentUI; }
    CUI* GetFindChild(CUI* _parentUI, const wstring& _childUI);

    const vector<CUI*>& GetChildUI() { return m_vecChildUI; }
    Vec2 GetFinalPos() { return m_vFinalPos; }

    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }

    void AddChild(CUI* _pUI) { m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this; }


private:
    void Update_Child();
    void FinalUpdate_Child();
    void Render_Child(HDC _dc);

    void MouseOnCheck();

public:
    virtual void MouseOn();
    virtual void MouseLBtnDown();
    virtual void MouseLBtnUp();
    virtual void MouseLBtnClicked();

public:
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render(HDC _dc);

public:
    virtual CUI* Clone() = 0;
};

// UI클래스 내부에는 이런 저런 UI들이 있기 때문에.. 이것을 '추상 클래스'로 만들어서 '객체화'할 수 없게 만들 것이다.
// 계층 구조의 장점: 자식 UI는 부모 UI가 반드시 먼저 Update가 되어야 그 데이터를 가져다가 본인의 상태를 결정 할 수 있음.
