#pragma once
#include "CUIPanel.h"

typedef struct _tagItemSlot
{
    ITEM_TAG    eItemTag;
    int         m_iCount;

    _tagItemSlot() :
        eItemTag(IT_NONE),
        m_iCount(0)
    {}
}SLOT, *PSLOT;

class CItem;
class CInventory :
    public CUIPanel
{
public:
    CInventory();
    virtual ~CInventory();

public:
    static SLOT m_tSlot[8][16];
    static bool m_bItemMove;

private:
    CItem* m_pItem[IT_SIZE];

    int m_iRow;
    int m_iCol;

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();

public:
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CInventory);
};