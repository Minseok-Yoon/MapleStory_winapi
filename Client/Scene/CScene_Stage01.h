#pragma once
#include "CScene.h"

class CTexture;

class CScene_Stage01 :
    public CScene
{
public:
    CScene_Stage01();
    virtual ~CScene_Stage01();

private:
    CTexture* m_pBackGround;

public:
    virtual void Update();
    virtual void Render(HDC _dc);

public:
    virtual void Enter();
    virtual void Exit();
};

