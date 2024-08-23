#pragma once

class CDamage;
class CTexture;

class CDamagerManager
{
	SINGLETON(CDamagerManager);

private:
	map<string, CTexture*> m_mapNoCriTexture; // ũ��Ƽ�� X �ؽ��� ��
	map<string, CTexture*> m_mapCriTexture; // ũ��Ƽ�� O �ؽ��� ��
    vector<CDamage*> m_vDamageInit;

public:
    CTexture* FindNumTexture(string strNum, bool bIsCri);
    void CreateDamage(int iDamage, Vec2& position, bool bIsCri, int i = 1);

    bool Init();
    void Update();
    void Render(HDC _dc);
};