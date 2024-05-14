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
	// 현재 작업 디렉터리 경로를 m_szContentPath에 저장한다.
	GetCurrentDirectory(255, m_szContentPath);
	// 현재 작업 디렉터리 경로의 길이를 구한다.
	int iLen = (int)wcslen(m_szContentPath);

	// 상위폴더로 이동하여 Content경로를 설정한다.
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

	// 절대 경로의 길이를 구한다.
	size_t iAbsLen = wcslen(m_szContentPath);
	// 전체 파일의 경로의 길이를 구한다.
	size_t iFullLen = strFilePath.length();

	// 절대 경로를 기준으로 상대 경로를 추출한다.
	wstring strRelativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);
	// 상대 경로를 반환한다.
	return strRelativePath;
}