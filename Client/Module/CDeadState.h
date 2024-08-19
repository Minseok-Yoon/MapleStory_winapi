#pragma once
#include "CState.h"

class CDeadState :
    public CState
{
public:
    CDeadState();
    ~CDeadState();

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};