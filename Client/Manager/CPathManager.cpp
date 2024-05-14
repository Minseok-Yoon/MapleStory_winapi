#include "../pch.h"
#include "CPathManager.h"
#include "../Core/CCore.h"

CPathManager::CPathManager() :
	m_szContentPath{}
{
}

CPathManager::~CPathManager()
{
}

void CPathManager::Init()
{
	// ���� �۾� ���͸� ��θ� m_szContentPath�� �����Ѵ�.
	GetCurrentDirectory(255, m_szContentPath);
	// ���� �۾� ���͸� ����� ���̸� ���Ѵ�.
	int iLen = (int)wcslen(m_szContentPath);

	// ���������� �̵��Ͽ� Content��θ� �����Ѵ�.
	for (int i = iLen - 1; i >= 0; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\bin\\Content\\");
}

wstring CPathManager::GetRelativePath(const wchar_t* _filepath)
{
	wstring strFilePath = _filepath;

	// ���� ����� ���̸� ���Ѵ�.
	size_t iAbsLen = wcslen(m_szContentPath);
	// ��ü ������ ����� ���̸� ���Ѵ�.
	size_t iFullLen = strFilePath.length();

	// ���� ��θ� �������� ��� ��θ� �����Ѵ�.
	wstring strRelativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);
	// ��� ��θ� ��ȯ�Ѵ�.
	return strRelativePath;
}