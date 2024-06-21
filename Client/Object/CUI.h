#pragma once
#include "CObject.h"

class CTexture;

class CUI :
    public CObject
{
    // CUIManager Ŭ������ CUIŬ������ private�� ������ �� �ֵ��� ���ش�.
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

    bool            m_bCamAffected; // UI�� ī�޶� ������ �޴� ����
    bool            m_bMouseOn;     // UI ���� ���콺�� �ִ���
    bool            m_bLbtnDown;    // UI�� ���ʹ�ư�� �������� �ִ���

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

// UIŬ���� ���ο��� �̷� ���� UI���� �ֱ� ������.. �̰��� '�߻� Ŭ����'�� ���� '��üȭ'�� �� ���� ���� ���̴�.
// ���� ������ ����: �ڽ� UI�� �θ� UI�� �ݵ�� ���� Update�� �Ǿ�� �� �����͸� �����ٰ� ������ ���¸� ���� �� �� ����.
