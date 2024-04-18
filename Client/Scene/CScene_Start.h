#pragma once
#include "CScene.h"

class CScene_Start :
    public CScene
{
public:
    CScene_Start();
    ~CScene_Start();

private:
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void Render(HDC _dc);
};