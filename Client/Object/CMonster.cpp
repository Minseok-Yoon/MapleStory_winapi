#include "../pch.h"
#include "CMonster.h"
#include "../Object/CObject.h"
#include "../Component/CCollider.h"
#include "../Module/AI.h"
#include "../Resource/CTexture.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CAnimator.h"

CMonster::CMonster()
    : m_tMonInfo(),
    m_fSpeed(100.f), // �ʱ� �ӵ� ����
    m_iDir(1), // �ʱ� �̵� ���� ���� (1: ������, -1: ����)
    m_vCenterPos(Vec2(500.f, 0.f)), // �ʱ� �߽� ��ġ
    m_fMaxDistance(200.f), // �ʱ� �ִ� ��ȸ �Ÿ�
    m_eCurMonState(MON_STATE::PATROL), // �ʱ� ���¸� PATROL�� ����
    m_ePrevMonState(MON_STATE::IDLE),
    m_fIdleTime(0.f), // ��� �ð� �ʱ�ȭ
    m_bWaiting(false) // ��� ���� �ʱ�ȭ
{
    MonsterAnimationClip();
}

CMonster::~CMonster()
{
    if (nullptr != m_pAI)
        delete m_pAI;
}

void CMonster::SetAI(AI* _AI)
{
    m_pAI = _AI;
    m_pAI->m_pMonster = this;
}

void CMonster::OnCollision(CCollider* _pOther)
{
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
}

void CMonster::OnCollisionExit(CCollider* _pOther)
{
}

void CMonster::Update_Animation()
{
    switch (m_eCurMonState)
    {
    case MON_STATE::IDLE:
        if (m_iDir == -1)
        {
            GetAnimator()->Play(L"StandLeft", true);
        }
        else
        {
            GetAnimator()->Play(L"StandRight", true);
        }
        break;

    case MON_STATE::PATROL:
        if (m_iDir == -1)
        {
            GetAnimator()->Play(L"WalkLeft", true);
        }
        else
        {
            GetAnimator()->Play(L"WalkRight", true);
        }
        break;

    case MON_STATE::TRACE:
        if (m_iDir == -1)
        {
            GetAnimator()->Play(L"WalkLeft", true);
        }
        else
        {
            GetAnimator()->Play(L"WalkRight", true);
        }
        break;

    default:
        OutputDebugStringA("Current State is not IDLE\n");
        break;
    }

    m_ePrevMonState = m_eCurMonState;
    m_iPrevDir = m_iDir;
}

void CMonster::MonsterAnimationClip()
{
    CreateAnimator();

    AddAnimationClip(L"StandRight", L"texture\\Monster\\Idle\\Right\\%d.bmp", 2, 0.7f, 63.f, 58.f);
    AddAnimationClip(L"StandLeft", L"texture\\Monster\\Idle\\Left\\%d.bmp", 2, 0.7f, 63.f, 58.f);
    AddAnimationClip(L"WalkRight", L"texture\\Monster\\Walk\\Right\\%d.bmp", 3, 0.6f, 64.f, 64.f);
    AddAnimationClip(L"WalkLeft", L"texture\\Monster\\Walk\\Left\\%d.bmp", 3, 0.6f, 64.f, 64.f);
}

void CMonster::AddAnimationClip(const wstring& strKey, const wchar_t* pFilePath, int iFrameMax, float fAnimationLimitTime, float fFrameSizeX, float fFrameSizeY)
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

void CMonster::Update()
{
    if (nullptr != m_pAI)
        m_pAI->Update();

    Vec2 vMonCurPos = GetPos();

    if (m_eCurMonState == MON_STATE::PATROL)
    {
        // ���� �������� �ʴ� speed��ŭ �̵�
        vMonCurPos.x += m_fSpeed * m_iDir * fDeltaTime;

        // ��ȸ�Ÿ� ���ط��� �Ѿ���� Ȯ��
        float fDist = abs(m_vCenterPos.x - vMonCurPos.x) - m_fMaxDistance;

        if (0.f < fDist)
        {
            // ��ȸ �Ÿ��� �ʰ��� ���, ��� ���·� ��ȯ
            //m_iDir *= -1;
            m_fIdleTime = 4.f; // 4�� ��� �ð� ����
            m_bWaiting = true;
            m_eCurMonState = MON_STATE::IDLE;
        }
    }
    else if (m_eCurMonState == MON_STATE::IDLE)
    {
        // ��� ������ �� Ÿ�̸Ӹ� ���ҽ�Ŵ
        if (m_bWaiting)
        {
            m_fIdleTime -= fDeltaTime;

            if (m_fIdleTime <= 0.f)
            {
                // ��� �ð��� ������ �ٽ� ��ȸ ���·� ��ȯ
                m_bWaiting = false;
                m_eCurMonState = MON_STATE::PATROL;
                // ���� ��ȯ �� �ٷ� �̵����� �ʵ��� vMonCurPos ������Ʈ
                m_iDir *= -1;
            }
        }
    }

    SetPos(vMonCurPos);

    Update_Animation();
}

void CMonster::Render(HDC _dc)
{
    CObject::ComponentRender(_dc);

    Vec2 vPos = GetPos();
    vPos = CCamera::GetInst()->GetRenderPos(vPos);
}