#pragma once
#include "CUI.h"

class CUIPanel :
    public CUI
{
public:
    CUIPanel();
    virtual ~CUIPanel();

private:
    Vec2    m_vDragStart;

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();

public:
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CUIPanel);
};