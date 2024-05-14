#pragma once

class CPathManager
{
	SINGLETON(CPathManager);

private:
	wchar_t		m_szContentPath[256];	// ���� ��θ� ������ wchar_t�� �迭�̴�.
	wchar_t		m_szRelativePath[256];	// ��� ��θ� ������ wchar_t�� �迭�̴�.

public:
	void Init();
	const wchar_t* GetContentPath() { return m_szContentPath; }

	// ��� ��θ� ��� ���� �Լ�
	wstring GetRelativePath(const wchar_t* _filepath);
};