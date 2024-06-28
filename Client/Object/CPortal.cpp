#include "../pch.h"
#include "CPortal.h"
#include "CObject.h"
#include "../Component/CCollider.h"
#include "../Component/CAnimator.h"
#include "../Component/CAnimation.h"

int CPortal::m_iPortalCount = 0;

CPortal::CPortal()
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2{ 0.f, 0.f });
	GetCollider()->SetScale(Vec2{ 20.f, 20.f });
	GetCollider()->SetColTag("Portal 0");

    PortalAnimationClip();
}

CPortal::CPortal(const CPortal& _origin) :
	CObject(_origin)
{
}

CPortal::~CPortal()
{
}

void CPortal::StagePortals()
{
	m_stagePortals["Stage1"] = {
		{{150.f, 820.f}, "Stage2"}
	};

	m_portalToStageMap["Portal 0"] = "Stage2";
}

void CPortal::PortalAnimationClip()
{
	CreateAnimator();

	AddAnimationClip(L"Stand", L"texture\\Portal\\%d.bmp", 8, 1.f, 88.f, 217.f);

}

void CPortal::AddAnimationClip(const wstring& strKey, const wchar_t* pFilePath, int iFrameMax, float fAnimationLimitTime, float fFrameSizeX, float fFrameSizeY)
{
    vector<wstring> vecFile;

    for (int i = 1; i <= iFrameMax; i++)
    {
        wchar_t strFileName[MAX_PATH] = {};
        wsprintf(strFileName, pFilePath, i);
        vecFile.push_back(strFileName);
    }

    GetAnimator()->CreateFrameAnimation(strKey, vecFile, Vec2(0.f, 0.f), Vec2(fFrameSizeX, fFrameSizeY), fAnimationLimitTime);
}

void CPortal::Update()
{
}

void CPortal::Render(HDC _dc)
{
    CObject::ComponentRender(_dc);

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
}