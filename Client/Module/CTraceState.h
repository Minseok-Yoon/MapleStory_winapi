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
    bool m_bReverseMove; // �ݴ� �������� �̵� ������ üũ�ϴ� �÷���
    float m_fReverseMoveTime; // �ݴ� �������� �̵��� �ð��� �����ϱ� ���� ����
    float m_fReverseMoveDuration; // �ݴ� �������� �̵��ϴ� �ð�

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};