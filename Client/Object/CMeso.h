#pragma once
#include "CItem.h"
class CMeso :
    public CItem
{
public:
    CMeso();
    virtual ~CMeso();

private:
    int     m_iMoneyAmount;
    CTexture* m_pTex;

public:
    void SetMoneyAmount(int _iAmount) { m_iMoneyAmount = _iAmount; }
    int GeMoneyAmount() const { return m_iMoneyAmount; }

public:
    virtual void Update();
    virtual void Render(HDC _dc);
};