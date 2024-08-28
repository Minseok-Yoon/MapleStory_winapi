#pragma once
#include "CScene.h"

class CTexture;

class CScene_Stage02 :
    public CScene
{
public:
    CScene_Stage02();
    virtual ~CScene_Stage02();

private:
    CTexture* m_pBackGround;

public:
    virtual void Update();
    virtual void Render(HDC _dc);

public:
    virtual void Enter();
    virtual void Exit();
};