#pragma once
#include "CState.h"

class CTraceState :
    public CState
{
public:
    CTraceState();
    ~CTraceState();

private:
    float   m_fLostTime;
    const float m_fMaxLostTime;

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};