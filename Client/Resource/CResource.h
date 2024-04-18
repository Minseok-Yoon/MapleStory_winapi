#pragma once

class CResource
{
public:
	CResource();
	~CResource();

private:
	wstring		m_strKey;
	wstring		m_strRelativePath;

public:
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	const wstring& GetKey() { return m_strKey; }

	void SetRelativePath(const wstring& _strPath) { m_strRelativePath = _strPath; }
	const wstring& GetRelativePath() { return m_strRelativePath; }
};