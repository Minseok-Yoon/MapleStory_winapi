#include "../pch.h"
#include "CMonster.h"
#include "../Object/CObject.h"
#include "../Component/CCollider.h"
#include "../Component/CGravity.h"
#include "../Module/AI.h"
#include "../Manager/CEventManager.h"
#include "../Resource/CTexture.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CAnimator.h"
#include "../Component/CColliderPixel.h"
#include "CItem.h"
#include "CMeso.h"
#include "../Scene/CScene.h"
#include "../Manager/CSceneManager.h"

CMonster::CMonster()
    : m_tMonInfo(),
    m_fSpeed(100.f), // �ʱ� �ӵ� ����
    m_iDir(1), // �ʱ� �̵� ���� ���� (1: ������, -1: ����)
    m_vCenterPos(Vec2(300.f, 0.f)), // �ʱ� �߽� ��ġ
    m_fMaxDistance(200.f), // �ʱ� �ִ� ��ȸ �Ÿ�
    m_eCurMonState(MON_STATE::PATROL), // �ʱ� ���¸� PATROL�� ����
    m_ePrevMonState(MON_STATE::IDLE),
    m_fIdleTime(0.f), // ��� �ð� �ʱ�ȭ
    m_bWaiting(false) // ��� ���� �ʱ�ȭ
{
    AddCollider();
    auto monsterCollider = GetCollider().back();
    monsterCollider->SetColTag("Monster");
    monsterCollider->SetOffsetPos(Vec2{ 0.f, 0.f });
    monsterCollider->SetScale(Vec2{ 20.f, 20.f });
    monsterCollider->SetLayer(1);  // Player ���̾� ����

    MonsterAnimationClip();

    CreateGravity();

    srand(static_cast<unsigned int>(time(nullptr)));
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

void CMonster::OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageColl")
    {
        OutputDebugStringA("Monster! Magenta Pixel Collidering!\n");
        Vec2 vPos = GetPos();

        GetGravity()->SetOnGround(true);
    }

    if (_pOther->GetColTag() == "Attack")
    {
        m_iHP -= 1;
        if (m_iHP <= 0 && m_eCurMonState != MON_STATE::DEAD)
        {
            OutputDebugStringA("Monster is dead!\n");
            //DeleteObject(this);
            m_eCurMonState = MON_STATE::DEAD;
            Sleep(3.5);
            DropItem();
            DeleteObject(this);
        }
    }
}

void CMonster::DropItem()
{
    CScene* pScene = CSceneManager::GetInst()->GetCurScene();
    Vec2 vPos = GetPos();

    if (pScene)
    {
        // 50% Ȯ���� ������ ���
        int itemDropChance = std::rand() % 100; // 0-99 ������ ���� ���� ����
        if (itemDropChance < 100)
        {
            CItem* pItem = new CItem();
            Vec2 vItemOffset = Vec2(-10.f, 0.f); // �������� ������ ���� (�������� 10����)
            Vec2 vItemPos = vPos + vItemOffset;
            pItem->SetItemInfo(vItemPos); // ������ ��ġ���� �������� ���Ͽ� ����
            pScene->AddObject(pItem, OBJECT_TYPE::ITEM);
            OutputDebugStringA("Item dropped!\n");
        }

        // 100% Ȯ���� �� ���
        CMeso* pMoney = new CMeso();
        Vec2 vMoneyOffset = Vec2(20.f, 0.f); // ���� ������ ���� (���������� 20����)
        Vec2 vMoneyPos = vPos + vMoneyOffset;
        pMoney->SetItemInfo(vMoneyPos); // ������ ��ġ���� �������� ���Ͽ� ����
        pMoney->SetMoneyAmount(100);
        pScene->AddObject(pMoney, OBJECT_TYPE::MESO);
        OutputDebugStringA("Money dropped!\n");
    }
    else
    {
        OutputDebugStringA("Failed to drop item or money: Scene not set.\n");
    }
}

void CMonster::OnCollision(CCollider* _ColTag, CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageColl")
    {
        Vec2 vPos = GetPos();

        GetGravity()->SetOnGround(true);
    }
}

void CMonster::OnCollisionExit(CCollider* _ColTag, CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageColl")
    {
        OutputDebugStringA("Collision Exit!\n");
        GetGravity()->SetOnGround(false);
    }
}

void CMonster::CheckPixelColor()
{
    if (m_pPixelCollider) {
        Vec2 vPos = GetPos();

        int playerHeightHalf = 32; // ��: �÷��̾� ������ ����
        int offsetY = playerHeightHalf;

        // ��ǥ ��ȯ�� �߰��մϴ�. (�ʿ信 ���� ��ȯ ��� ����)
        int x = static_cast<int>(vPos.x);
        int y = static_cast<int>(vPos.y + offsetY);

        // ��ǥ ��ȯ�� �߰��մϴ�. (�ʿ信 ���� ��ȯ ��� ����)
        if (x >= 0 && x < m_pPixelCollider->GetWidth() && y >= 0 && y < m_pPixelCollider->GetHeight()) {
            PIXEL pixel = m_pPixelCollider->GetPixelColor(x, y);

            for (auto collider : GetCollider())
            {
                if (collider && collider->GetLayer() == 1) {  // Player ���̾����� Ȯ��
                    if (pixel.r == 255 && pixel.g == 0 && pixel.b == 255) {
                        if (collider->GetColTag() != "StageColl") {
                            // ���� �±װ� StageColl�� �ƴϸ� ���ο� ���� Collider�� �浹 ó��
                            CCollider tempCollider;
                            tempCollider.SetColTag("StageColl");
                            OnCollisionEnter(collider, &tempCollider);
                        }
                    }
                    else {
                        if (collider->GetColTag() == "StageColl") {
                            CCollider tempCollider;
                            tempCollider.SetColTag("StageColl");
                            OnCollisionExit(collider, &tempCollider);
                        }
                    }
                }
            }
        }
        else {
            OutputDebugStringA("Player position out of bounds.\n");
        }
    }
    else {
        OutputDebugStringA("Pixel collider not set.\n");
    }
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

    case MON_STATE::DEAD:
        if (m_iDir == -1)
        {
            GetAnimator()->Play(L"DeadLeft", false);
        }
        else
        {
            GetAnimator()->Play(L"DeadRight", false);
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
    AddAnimationClip(L"DeadRight", L"texture\\Monster\\Dead\\Right\\%d.bmp", 3, 0.5f, 61.f, 59.f);
    AddAnimationClip(L"DeadLeft", L"texture\\Monster\\Dead\\Left\\%d.bmp", 3, 0.5f, 61.f, 59.f);
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

    else if (m_eCurMonState == MON_STATE::TRACE)
    {
    }

    SetPos(vMonCurPos);

    CheckPixelColor();

    Update_Animation();
}

void CMonster::Render(HDC _dc)
{
    CObject::ComponentRender(_dc);

    Vec2 vPos = GetPos();
    vPos = CCamera::GetInst()->GetRenderPos(vPos);
}