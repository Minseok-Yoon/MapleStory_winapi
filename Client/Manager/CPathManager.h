#pragma once

class CPathManager
{
	SINGLETON(CPathManager);

private:
	wchar_t		m_szContentPath[256];	// 절대 경로를 저장할 wchar_t형 배열이다.
	wchar_t		m_szRelativePath[256];	// 상대 경로를 저장할 wchar_t형 배열이다.

public:
	void Init();
	const wchar_t* GetContentPath() { return m_szContentPath; }

	// 상대 경로를 얻기 위한 함수
	wstring GetRelativePath(const wchar_t* _filepath);
};