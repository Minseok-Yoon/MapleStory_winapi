#pragma once

class CResource;
class CTexture;

class CResourceManager
{
	SINGLETON(CResourceManager);
private:
	map<wstring, CResource*>	m_mapTex;

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
	CTexture* CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight);
	CTexture* FindTexture(const wstring& _strKey);
};