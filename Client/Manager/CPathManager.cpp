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
	GetCurrentDirectory(255, m_szContentPath);

	int iLen = (int)wcslen(m_szContentPath);

	// 상위폴더로
	for (int i = iLen - 1; i >= 0; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\bin\\Content\\");

	SetWindowText(CCore::GetInst()->GetMainHWnd(), m_szContentPath);
}

wstring CPathManager::GetRelativePath(const wchar_t* _filepath)
{
	wstring strFilePath = _filepath;

	size_t iAbsLen = wcslen(m_szContentPath);
	size_t iFullLen = strFilePath.length();

	wstring strRelativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);

	return strRelativePath;
}