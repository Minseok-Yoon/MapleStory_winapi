#pragma once
#include "CState.h"

class CTraceState :
    public CState
{
public:
    CTraceState();
    ~CTraceState();

private:
    float       m_fLostTime;
    const float m_fMaxLostTime;
    bool m_bReverseMove; // 반대 방향으로 이동 중인지 체크하는 플래그
    float m_fReverseMoveTime; // 반대 방향으로 이동한 시간을 측정하기 위한 변수
    float m_fReverseMoveDuration; // 반대 방향으로 이동하는 시간

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};