#pragma once

class CDamage;
class CTexture;

class CDamageManager
{
    SINGLETON(CDamageManager);

private:
    map<string, CTexture*> m_mapNoCriTexture; // ũ��Ƽ�� X �ؽ��� ��
    map<string, CTexture*> m_mapCriTexture; // ũ��Ƽ�� O �ؽ��� ��
    vector<CDamage*> m_vDamageInit;

public:
    CTexture* FindNumTexture(string _strNum, bool _bIsCri);
    void CreateDamage(int _iDamage, Vec2& _position, bool _bIsCri, int i = 1);

    bool Init();
    void Update();
    void Render(HDC _dc);
};