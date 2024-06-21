#pragma once
#include "CScene.h"

class CUI;
class CBtnUI;
class CTexture;

class CScene_Start :
    public CScene
{
public:
    CScene_Start();
    virtual ~CScene_Start();

private:
    CUI* m_pUI;
    CTexture* m_pTexture;

private:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void Render(HDC _dc);
};