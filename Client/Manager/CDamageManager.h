#pragma once

class CDamage;
class CTexture;

class CDamageManager
{
    SINGLETON(CDamageManager);

private:
    map<string, CTexture*> m_mapNoCriTexture; // 크리티컬 X 텍스쳐 맵
    map<string, CTexture*> m_mapCriTexture; // 크리티컬 O 텍스쳐 맵
    vector<CDamage*> m_vDamageInit;

public:
    CTexture* FindNumTexture(string _strNum, bool _bIsCri);
    void CreateDamage(int _iDamage, Vec2& _position, bool _bIsCri, int i = 1);

    bool Init();
    void Update();
    void Render(HDC _dc);
};