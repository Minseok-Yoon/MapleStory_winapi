#include "../pch.h"
#include "CColliderPixel.h"
#include "../Manager/CPathManager.h"

CColliderPixel::CColliderPixel()
{
	m_iWidth = 0;
	m_iHeight = 0;
}

CColliderPixel::CColliderPixel(const CColliderPixel& _origin)
{
	m_iWidth = _origin.m_iWidth;
	m_iHeight = _origin.m_iHeight;
}

CColliderPixel::~CColliderPixel()
{
}

bool CColliderPixel::SetPixelInfo(const wstring& _pFileName)
{
	wstring filePath = CPathManager::GetInst()->GetContentPath();
	filePath += _pFileName;

	FILE* file = nullptr;

	_wfopen_s(&file, filePath.c_str(), L"rb");
	// 파일 열기 실패
	assert(file);

	BITMAPFILEHEADER	fh;
	BITMAPINFOHEADER	ih;

	fread(&fh, sizeof(fh), 1, file);
	fread(&ih, sizeof(ih), 1, file);

	m_iWidth = ih.biWidth;
	m_iHeight = ih.biHeight;

	m_vecPixel.clear();

	m_vecPixel.resize(m_iWidth * m_iHeight);

	fread(&m_vecPixel[0], sizeof(PIXEL), m_vecPixel.size(), file);

	fclose(file);

	PPIXEL pPixelArr = new PIXEL[m_iWidth];		// 한줄짜리 픽셀 배열

	// 위 아래를 반전시켜준다.
	for (int i = 0; i < m_iHeight / 2; ++i)
	{
		// 현재 인덱스의 픽셀 한 줄을 저장해둔다.
		memcpy(pPixelArr, &m_vecPixel[i * m_iWidth], sizeof(PIXEL) * m_iWidth);
		memcpy(&m_vecPixel[i * m_iWidth], &m_vecPixel[(m_iHeight - i - 1) * m_iWidth],
			sizeof(PIXEL) * m_iWidth);
		memcpy(&m_vecPixel[(m_iHeight - i - 1) * m_iWidth], pPixelArr,
			sizeof(PIXEL) * m_iWidth);
	}

	delete[] pPixelArr;

	fclose(file);
	OutputDebugStringA("Pixel data loaded successfully.\n");

	return true;
}

PIXEL CColliderPixel::GetPixelColor(int x, int y)
{
	if (x < 0 || x >= m_iWidth || y < 0 || y >= m_iHeight) {
		return { 0, 0, 0 }; // 유효하지 않은 좌표일 경우 검정색 반환
	}
	return m_vecPixel[y * m_iWidth + x];
}

void CColliderPixel::FinalUpdate()
{
}

void CColliderPixel::Render(HDC _dc)
{
}
