#pragma once
#include "CObject.h"

class CPortal :
    public CObject
{
    friend class CObject;

public:
    CPortal(const string& portalTag);
    CPortal(const CPortal& _origin);
    ~CPortal();

public:
    unordered_map<string, vector<pair<pair<float, float>, string>>> m_stagePortals;
    unordered_map<string, wstring> m_stageFiles;
    unordered_map<string, string>  m_portalToStageMap;


private:
    pair<float, float> m_tPosID;    // ��ġ �ĺ���
    static int  m_iPortalCount;
    string      m_portalId;         // ��Ż�� ������ �ĺ� ��
    string      m_sTargetStage;     // ��ǥ ���������� �̸��� ����
    string     m_strPortalTag; // ��Ż �±׸� �����ϴ� ��� ����
 

public:
    void SetPosID(pair<float, float> posID) { m_tPosID = posID; }
    pair<float, float> GetPosID()   const { return m_tPosID; }

    void SetTargetStage(const string& sTargetStage)
    {
        m_sTargetStage = sTargetStage;
    }

    const string& GetTargetStage()  const
    {
        return m_sTargetStage;
    }

    void SetPortalID(string id)
    {
        m_portalId = id;
    }

    string GetPortalID() const
    {
        return m_portalId;
    }

    void SetPortalTag(const string& _strPortalTag) { m_strPortalTag = _strPortalTag; }
    const string& GetPortalTag() const { return m_strPortalTag; }

public:
    void StagePortals();
    void PortalAnimationClip();

    void AddAnimationClip(const wstring& strKey, const wchar_t* pFilePath, int iFrameMax, float fAnimationLimitTime, float fFrameSizeX, float fFrameSizeY, float _fOffsetX, float _fOffsetY);

public:
    virtual void FastUpdate() {}; 	// Scene�� ���۵Ǳ� ������ ȣ��Ǵ� �Լ�
    virtual void Update();
    virtual void Render(HDC _dc);

    CLONE(CPortal);
};