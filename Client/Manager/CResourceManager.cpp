#include "../pch.h"
#include "CResourceManager.h"
#include "CPathManager.h"
#include "../Resource/CTexture.h"
#include "../Resource/CResource.h"

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
    Safe_Delete_Map(m_mapTex);
}

CTexture* CResourceManager::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
    CTexture* pTexture = FindTexture(_strKey);
    if (nullptr != pTexture)
    {
        return pTexture;
    }

    wstring strFilePath = CPathManager::GetInst()->GetContentPath();
    strFilePath += _strRelativePath;

    pTexture = new CTexture;
    pTexture->Load(strFilePath);
    pTexture->SetKey(_strKey);
    pTexture->SetRelativePath(_strRelativePath);

    m_mapTex.insert(make_pair(_strKey, pTexture));

    return pTexture;
}

CTexture* CResourceManager::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight)
{
    CTexture* pTexture = FindTexture(_strKey);
    if (nullptr != pTexture)
    {
        return pTexture;
    }

    pTexture = new CTexture;
    pTexture->Create(_iWidth, _iHeight);
    pTexture->SetKey(_strKey);

    m_mapTex.insert(make_pair(_strKey, pTexture));

    return pTexture;
}

CTexture* CResourceManager::FindTexture(const wstring& _strKey)
{
    map<wstring, CResource*>::iterator iter = m_mapTex.find(_strKey);

    if (iter == m_mapTex.end())
    {
        return nullptr;
    }

    return (CTexture*)iter->second;
}
