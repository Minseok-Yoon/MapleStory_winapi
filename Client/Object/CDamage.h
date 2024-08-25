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
    vector<CTexture*> m_vecDamage; // 데미지 텍스쳐
    vector<Vec2> m_vecPos; // 위치
    int iLength; // 데미지 길이
    float fDelay; // 데미지 딜레이
    bool bEnable; // 딜레이가 끝나면 활성화시켜줄 플래그
    float fExistTime; // 존재한 시간

public:
    void Init(int _iDamage, Vec2& _vPosition, bool _bIsCri, CDamageManager* _pManager, int i = 1);
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CDamage);
};