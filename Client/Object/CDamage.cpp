#include "../pch.h"
#include "CDamage.h"
#include "../Manager/CDamageManager.h"
#include "../Resource/CTexture.h"

CDamage::CDamage()	:
    iLength(0),
    fDelay(0.f),
    bEnable(false),
    fExistTime(0.f)
{
}

CDamage::~CDamage()
{
}

void CDamage::Init(int _iDamage, Vec2& _vPosition, bool _bIsCri, CDamageManager* _pManager, int i)
{
    fDelay = i * 0.1f;
    bEnable = false;
    fExistTime = 0.f;

    string strDamage = to_string(_iDamage);
    iLength = strDamage.length();
    Vec2 tFirstPos = Vec2(_vPosition.x - (float)iLength * 20.f / 2.f, _vPosition.y - 100.f - (i - 1) * 26.f);

    for (int j = 0; j < iLength; ++j)
    {
        CTexture* pTex = _pManager->FindNumTexture(strDamage.substr(j, 1), _bIsCri);
        if (!pTex) printf("Failed to find texture for digit: %c\n", strDamage[j]);
        m_vecDamage.push_back(pTex);
        Vec2 tPos = Vec2(tFirstPos.x + j * 20.f, tFirstPos.y + 8.f - (float)(j % 2));
        m_vecPos.push_back(tPos);
    }
}

void CDamage::Update()
{
    if (bEnable)
    {
        // 데미지 위치 업데이트 (여기서는 y 위치만 변경)
        for (int i = 0; i < iLength; ++i)
        {
            m_vecPos[i].y -= 1.f; // y축으로 이동, deltaTime 없이 고정된 값 사용
        }

        // 존재 시간을 증가시키고, 1초 후 비활성화
        fExistTime += 0.016f; // 약 60FPS 기준, 고정 시간 증가
        if (fExistTime >= 1.f)
        {
            bEnable = false;
        }
    }
    else
    {
        // 지연 시간이 끝나면 활성화
        fDelay -= 0.016f;
        if (fDelay <= 0.f)
        {
            bEnable = true;
        }
    }
}

void CDamage::Render(HDC _dc)
{
    if (bEnable)
    {
        for (int i = 0; i < iLength; ++i)
        {
            CTexture* pTexture = m_vecDamage[i];
            Vec2 position = m_vecPos[i];

            if (pTexture)
            {
                // 텍스처를 position 위치에 렌더링
                int iWidth = (int)pTexture->Width();
                int iHeight = (int)pTexture->Height();

                TransparentBlt(_dc,
                    int(position.x - (float)(iWidth / 2)),
                    int(position.y - (float)(iHeight / 2)),
                    iWidth, iHeight,
                    pTexture->GetDC(),
                    0, 0, iWidth, iHeight,
                    RGB(255, 255, 255));
            }
        }
    }
}