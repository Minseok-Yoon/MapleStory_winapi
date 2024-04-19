#pragma once
#include "CScene.h"

class CScene_Stage01 :
    public CScene
{
public:
    CScene_Stage01();
    ~CScene_Stage01();

public:
    virtual void Update();
    virtual void Render(HDC _dc);

public:
    virtual void Enter();
    virtual void Exit();
};

