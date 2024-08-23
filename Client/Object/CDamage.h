#pragma once
#include "CObject.h"

class CDamagerManager;

class CDamage :
    public CObject
{
public:
    CDamage();
    ~CDamage();

public:
    vector<CTexture*> m_vecDamage; // ������ �ؽ���
    vector<Vec2> m_vecPos; // ��ġ
    int iLength; // ������ ����
    float fDelay; // ������ ������
    bool bEnable; // �����̰� ������ Ȱ��ȭ������ �÷���
    float fExistTime; // ������ �ð�

public:
    void Init(int iDamage, Vec2& vPosition, bool bIsCri, CDamagerManager* pManager, int i = 1);
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CDamage);
};