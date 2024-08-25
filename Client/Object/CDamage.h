#pragma once
#include "CObject.h"

class CDamageManager;

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
    void Init(int _iDamage, Vec2& _vPosition, bool _bIsCri, CDamageManager* _pManager, int i = 1);
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CDamage);
};