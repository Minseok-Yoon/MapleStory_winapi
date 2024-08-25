#include "../pch.h"
#include "CDamageManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"
#include "../Scene/CScene.h"
#include "../Object/CObject.h"
#include "../Object/CDamage.h"

CDamageManager::CDamageManager()
{
}

CDamageManager::~CDamageManager()
{
}

CTexture* CDamageManager::FindNumTexture(string _strNum, bool _bIsCri)
{
    map<string, CTexture*>::iterator iter;

    if (_bIsCri)
    {
        iter = m_mapCriTexture.find(_strNum);
        if (iter == m_mapCriTexture.end())
        {
            return NULL;
        }
        return iter->second;
    }
    else
    {
        iter = m_mapNoCriTexture.find(_strNum);
        if (iter == m_mapNoCriTexture.end())
        {
            return NULL;
        }

        return iter->second;
    }
}

void CDamageManager::CreateDamage(int _iDamage, Vec2& _position, bool _bIsCri, int i)
{
    CDamage* pDamage = new CDamage();
    pDamage->fDelay = i * 0.1f;
    pDamage->bEnable = true;
    pDamage->fExistTime = 0.f;

    string strDamage = to_string(_iDamage);
    pDamage->iLength = strDamage.length();

    Vec2 tFirstPos = Vec2(_position.x - (float)pDamage->iLength * 20.f / 2.f,
        _position.y - 300.f - (i - 1) * 26.f);

    CTexture* pFirstTex = FindNumTexture(strDamage.substr(0, 1), _bIsCri);
    pDamage->m_vecDamage.push_back(pFirstTex);
    pDamage->m_vecPos.push_back(tFirstPos);

    for (int i = 1; i < pDamage->iLength; i++)
    {
        CTexture* pTex = FindNumTexture(strDamage.substr(i, 1), _bIsCri);
        pDamage->m_vecDamage.push_back(pTex);
        Vec2 tPos = Vec2(tFirstPos.x + i * 20.f, tFirstPos.y + 8.f - (float)(i % 2));
        pDamage->m_vecPos.push_back(tPos);
    }

    m_vDamageInit.push_back(pDamage);
}

bool CDamageManager::Init()
{
    // NoCri 텍스처 로드
    m_mapNoCriTexture["0"] = CResourceManager::GetInst()->LoadTexture(L"NoCri0", L"texture\\Damage\\NoCri\\0.bmp");
    if (!m_mapNoCriTexture["0"]) OutputDebugStringA("Failed to load NoCri0 texture\n");
    m_mapNoCriTexture["1"] = CResourceManager::GetInst()->LoadTexture(L"NoCri1", L"texture\\Damage\\NoCri\\1.bmp");
    if (!m_mapNoCriTexture["1"]) OutputDebugStringA("Failed to load NoCri1 texture\n");
    m_mapNoCriTexture["2"] = CResourceManager::GetInst()->LoadTexture(L"NoCri2", L"texture\\Damage\\NoCri\\2.bmp");
    if (!m_mapNoCriTexture["2"]) OutputDebugStringA("Failed to load NoCri2 texture\n");
    m_mapNoCriTexture["3"] = CResourceManager::GetInst()->LoadTexture(L"NoCri3", L"texture\\Damage\\NoCri\\3.bmp");
    if (!m_mapNoCriTexture["3"]) OutputDebugStringA("Failed to load NoCri3 texture\n");
    m_mapNoCriTexture["4"] = CResourceManager::GetInst()->LoadTexture(L"NoCri4", L"texture\\Damage\\NoCri\\4.bmp");
    if (!m_mapNoCriTexture["4"]) OutputDebugStringA("Failed to load NoCri4 texture\n");
    m_mapNoCriTexture["5"] = CResourceManager::GetInst()->LoadTexture(L"NoCri5", L"texture\\Damage\\NoCri\\5.bmp");
    if (!m_mapNoCriTexture["5"]) OutputDebugStringA("Failed to load NoCri5 texture\n");
    m_mapNoCriTexture["6"] = CResourceManager::GetInst()->LoadTexture(L"NoCri6", L"texture\\Damage\\NoCri\\6.bmp");
    if (!m_mapNoCriTexture["6"]) OutputDebugStringA("Failed to load NoCri6 texture\n");
    m_mapNoCriTexture["7"] = CResourceManager::GetInst()->LoadTexture(L"NoCri7", L"texture\\Damage\\NoCri\\7.bmp");
    if (!m_mapNoCriTexture["7"]) OutputDebugStringA("Failed to load NoCri7 texture\n");
    m_mapNoCriTexture["8"] = CResourceManager::GetInst()->LoadTexture(L"NoCri8", L"texture\\Damage\\NoCri\\8.bmp");
    if (!m_mapNoCriTexture["8"]) OutputDebugStringA("Failed to load NoCri8 texture\n");
    m_mapNoCriTexture["9"] = CResourceManager::GetInst()->LoadTexture(L"NoCri9", L"texture\\Damage\\NoCri\\9.bmp");
    if (!m_mapNoCriTexture["9"]) OutputDebugStringA("Failed to load NoCri9 texture\n");

    // Cri 텍스처 로드
    m_mapCriTexture["0"] = CResourceManager::GetInst()->LoadTexture(L"Cri0", L"texture\\Damage\\Cri\\0.bmp");
    if (!m_mapNoCriTexture["0"]) OutputDebugStringA("Failed to load Cri0 texture\n");
    m_mapCriTexture["1"] = CResourceManager::GetInst()->LoadTexture(L"Cri1", L"texture\\Damage\\Cri\\1.bmp");
    if (!m_mapNoCriTexture["1"]) OutputDebugStringA("Failed to load Cri1 texture\n");
    m_mapCriTexture["2"] = CResourceManager::GetInst()->LoadTexture(L"Cri2", L"texture\\Damage\\Cri\\2.bmp");
    if (!m_mapNoCriTexture["2"]) OutputDebugStringA("Failed to load Cri2 texture\n");
    m_mapCriTexture["3"] = CResourceManager::GetInst()->LoadTexture(L"Cri3", L"texture\\Damage\\Cri\\3.bmp");
    if (!m_mapNoCriTexture["3"]) OutputDebugStringA("Failed to load Cri3 texture\n");
    m_mapCriTexture["4"] = CResourceManager::GetInst()->LoadTexture(L"Cri4", L"texture\\Damage\\Cri\\4.bmp");
    if (!m_mapNoCriTexture["4"]) OutputDebugStringA("Failed to load Cri4 texture\n");
    m_mapCriTexture["5"] = CResourceManager::GetInst()->LoadTexture(L"Cri5", L"texture\\Damage\\Cri\\5.bmp");
    if (!m_mapNoCriTexture["5"]) OutputDebugStringA("Failed to load Cri5 texture\n");
    m_mapCriTexture["6"] = CResourceManager::GetInst()->LoadTexture(L"Cri6", L"texture\\Damage\\Cri\\6.bmp");
    if (!m_mapNoCriTexture["6"]) OutputDebugStringA("Failed to load Cri6 texture\n");
    m_mapCriTexture["7"] = CResourceManager::GetInst()->LoadTexture(L"Cri7", L"texture\\Damage\\Cri\\7.bmp");
    if (!m_mapNoCriTexture["7"]) OutputDebugStringA("Failed to load Cri7 texture\n");
    m_mapCriTexture["8"] = CResourceManager::GetInst()->LoadTexture(L"Cri8", L"texture\\Damage\\Cri\\8.bmp");
    if (!m_mapNoCriTexture["8"]) OutputDebugStringA("Failed to load Cri8 texture\n");
    m_mapCriTexture["9"] = CResourceManager::GetInst()->LoadTexture(L"Cri9", L"texture\\Damage\\Cri\\9.bmp");
    if (!m_mapNoCriTexture["9"]) OutputDebugStringA("Failed to load Cri9 texture\n");

    return true;
}

void CDamageManager::Update()
{
    for (auto iter = m_vDamageInit.begin(); iter != m_vDamageInit.end();) {
        CDamage* pDamage = *iter;

        // bEnable이 true인 경우에만 Update를 호출하도록 함
        if (pDamage->bEnable) {
            pDamage->Update();
        }

        // 존재 시간이 1초를 넘으면 객체 삭제
        if (!pDamage->bEnable && pDamage->fExistTime >= 1.f) {
            iter = m_vDamageInit.erase(iter);
            delete pDamage;
        }
        else {
            ++iter;
        }
    }
}

void CDamageManager::Render(HDC _dc)
{
    for (auto pDamage : m_vDamageInit) {
        if (pDamage->bEnable) {
            // 디버깅 메시지 준비
            char buffer[256];  // 충분한 크기의 버퍼를 사용
            sprintf_s(buffer, "Rendering damage at position x: %f, y: %f\n",
                pDamage->m_vecPos[0].x, pDamage->m_vecPos[0].y);

            // 디버깅 메시지 출력
            OutputDebugStringA(buffer);

            // 렌더링 호출
            pDamage->Render(_dc);
        }
    }
}