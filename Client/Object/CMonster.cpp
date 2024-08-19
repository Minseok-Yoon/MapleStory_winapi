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
#include "../Component/CAnimation.h"
#include "../Component/CColliderPixel.h"
#include "CItem.h"
#include "CMeso.h"
#include "../Scene/CScene.h"
#include "../Manager/CSceneManager.h"
#include "../Component/CRigidBody.h"

CMonster::CMonster(const Vec2& initialPos)
    : m_tMonInfo(),
    m_fSpeed(100.f), // �ʱ� �ӵ� ����
    m_iDir(1), // �ʱ� �̵� ���� ���� (1: ������, -1: ����)
    m_vCenterPos(initialPos), // �ʱ� �߽� ��ġ
    m_fMaxDistance(200.f), // �ʱ� �ִ� ��ȸ �Ÿ�
    m_eCurMonState(MON_STATE::PATROL), // �ʱ� ���¸� PATROL�� ����
    m_ePrevMonState(MON_STATE::PATROL),
    m_fIdleTime(0.f), // ��� �ð� �ʱ�ȭ
    m_bWaiting(false), // ��� ���� �ʱ�ȭ
    m_bDeadAniFin(false)
{
    AddCollider();
    auto monsterCollider = GetCollider().back();
    monsterCollider->SetColTag("Monster");
    monsterCollider->SetOffsetPos(Vec2{ 0.f, 0.f });
    monsterCollider->SetScale(Vec2{ 20.f, 20.f });
    monsterCollider->SetLayer(1);

    MonsterAnimationClip();

    CreateGravity();
    CreateRigidBody();

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

void CMonster::ReduceHP(int _damage)
{
    m_iHP -= _damage;
    if (m_iHP <= 0 && m_eCurMonState != MON_STATE::DEAD)
    {
        OnDeath(); // ü���� 0 ������ �� ��� ó��
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
            pItem->SetPos(vItemPos); // ������ ��ġ���� �������� ���Ͽ� ����
            pScene->AddObject(pItem, OBJECT_TYPE::ITEM);
            OutputDebugStringA("Item dropped!\n");
        }

        // 100% Ȯ���� �� ���
        CMeso* pMoney = new CMeso();
        Vec2 vMoneyOffset = Vec2(20.f, 0.f); // ���� ������ ���� (���������� 20����)
        Vec2 vMoneyPos = vPos + vMoneyOffset;
        pMoney->SetPos(vMoneyPos); // ������ ��ġ���� �������� ���Ͽ� ����
        pMoney->SetMoneyAmount(100);
        pScene->AddObject(pMoney, OBJECT_TYPE::MESO);
        OutputDebugStringA("Money dropped!\n");
    }
    else
    {
        OutputDebugStringA("Failed to drop item or money: Scene not set.\n");
    }
}

void CMonster::OnDeath()
{
    // ���¸� DEAD�� ����
    m_eCurMonState = MON_STATE::DEAD;

    GetAnimator()->Play(m_iDir == -1 ? L"DeadLeft" : L"DeadRight", false); // �״� �ִϸ��̼� ��� (�ݺ� ����)
    m_bDeadAniFin = false; // �״� �ִϸ��̼� �Ϸ� �÷��� �ʱ�ȭ
}

void CMonster::CheckDeathAnimation()
{
    if (m_eCurMonState == MON_STATE::DEAD && !m_bDeadAniFin)
    {
        // ���� ��� ���� �ִϸ��̼��� �Ϸ�Ǿ����� Ȯ��
        if (GetAnimator()->GetCurrentAnimation()->IsFinish())
        {
            m_bDeadAniFin = true; // �ִϸ��̼� �Ϸ� �÷��� ����
            DropItem();
            DeleteObject(this);
        }
    }
}

void CMonster::OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageColl")
    {
        OutputDebugStringA("Collision Enter with StageColl detected.\n");
        Vec2 vPos = GetPos();
        vPos.y = floor(vPos.y);
        SetPos(vPos);
        GetGravity()->SetOnGround(true);
    }

    /*if (_pOther->GetColTag() == "Attack")
    {
        m_iHP -= 1;
        if (m_iHP <= 0 && m_eCurMonState != MON_STATE::DEAD)
        {
            m_eCurMonState = MON_STATE::DEAD;
            OnDeath();
        }
    }*/
}

void CMonster::OnCollision(CCollider* _ColTag, CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageColl")
    {
        GetGravity()->SetOnGround(true);
        Vec2 vPos = GetPos();
        vPos.y = floor(vPos.y);
        SetPos(vPos);
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

void CMonster::OnWallCollisionEnter(CCollider* _ColTag, CCollider* _pOther)
{
    if (_pOther->GetColTag() == "Wall")
    {
        if (m_eCurMonState == MON_STATE::PATROL || m_eCurMonState == MON_STATE::TRACE) {
            m_iDir *= -1; // ���� ��ȯ
        }
    }
}

void CMonster::OnWallCollision(CCollider* _ColTag, CCollider* _pOther)
{
    if (_pOther->GetColTag() == "Wall")
    {
        if (m_eCurMonState == MON_STATE::PATROL || m_eCurMonState == MON_STATE::TRACE) {
            m_iDir *= -1; // ���� ��ȯ
        }
    }
}

void CMonster::OnWallCollisionExit(CCollider* _ColTag, CCollider* _pOther)
{

}

void CMonster::OnStageEndCollision(CCollider* _ColTag, CCollider* _pOther)
{
}

void CMonster::OnStageEndCollisionEnter(CCollider* _ColTag, CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageEnd")
    {
        if (m_eCurMonState == MON_STATE::PATROL || m_eCurMonState == MON_STATE::TRACE) {
            m_iDir *= -1; // ���� ��ȯ
        }
    }
}

void CMonster::OnStageEndCollisionExit(CCollider* _ColTag, CCollider* _pOther)
{
}

vector<Vec2> CMonster::GetCollisionPoint(const Vec2& _vPos, int _iMonHeightHalf)
{
    vector<Vec2> collisionPoint;
    int offsetY = _iMonHeightHalf;

    // ���� ��ġ�� ���� ��ǥ�� ���
    int x = static_cast<int>(_vPos.x);
    int y = static_cast<int>(_vPos.y + offsetY);

    // �¿� �浹 �˻� ��ǥ�� ���
    int xLeft = x - 15.f;
    int xRight = x + 15.f;

    int yPos = y - 30.f;

    // �浹 ��ǥ��  ���͸� �߰�
    collisionPoint.emplace_back(Vec2(x, y));
    collisionPoint.emplace_back(Vec2(xLeft, yPos));
    collisionPoint.emplace_back(Vec2(xRight, yPos));

    return collisionPoint;
}

bool CMonster::CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag)
{
    // ��ǥ�� �ȼ� �浹 ��ü�� ���� ���� �ִ��� Ȯ��
    if (_iPosX >= 0 && _iPosX < m_pPixelCollider->GetWidth() && _iPosY >= 0 && _iPosY < m_pPixelCollider->GetHeight()) {
        _pPixel = m_pPixelCollider->GetPixelColor(_iPosX, _iPosY);

        // �浹 �±׿� ���� �浹 ���θ� ����
        if (_colTag == "StageColl") {
            return (_pPixel.r == 255 && _pPixel.g == 0 && _pPixel.b == 255);
        }
        else if (_colTag == "Wall") {
            return (_pPixel.r == 0 && _pPixel.g == 255 && _pPixel.b == 255);
        }
        else if (_colTag == "StageEnd") {
            return (_pPixel.r == 128 && _pPixel.g == 0 && _pPixel.b == 0);
        }
    }
    else {
        OutputDebugStringA("Position out of bounds.\n");
    }
    return false;
}

void CMonster::UpdateCollisionState(bool& _bIsColiding, bool _bCollisionDetected, const string& _strColTag, void(CMonster::* onEnter)(), void(CMonster::* onExit)())
{
    if (_bCollisionDetected) {
        if (!_bIsColiding) {
            _bIsColiding = true;
            (this->*onEnter)();
        }
    }
    else {
        if (_bIsColiding) {
            _bIsColiding = false;
            (this->*onExit)();
        }
    }
}

void CMonster::CheckPixelColor()
{
    if (m_pPixelCollider) {
        Vec2 vPos = GetPos();

        int monHeightHalf = 26;

        auto collisionPoints = GetCollisionPoint(vPos, monHeightHalf);
        m_CollisionPoint.clear();

        // �浹 ���� �÷��� �ʱ�ȭ
        bool stageCollisionDetected = false;
        bool wallCollisionDetected = false;
        bool stageEndCollisionDectected = false;

        PIXEL pixel;
        for (const auto& point : collisionPoints) {
            if (CheckPixelCollision(point.x, point.y, pixel, "StageColl")) {
                m_CollisionPoint.emplace_back(point);
                stageCollisionDetected = true;
            }
        }

        // �� �浹 �˻�
        for (size_t i = 1; i <= 2; ++i) {
            const auto& point = collisionPoints[i];
            if (CheckPixelCollision(point.x, point.y, pixel, "Wall")) {
                m_CollisionPoint.emplace_back(point);
                wallCollisionDetected = true;
            }
        }

        // �� �浹 �˻�
        for (size_t i = 1; i <= 2; ++i) {
            const auto& point = collisionPoints[i];
            if (CheckPixelCollision(point.x, point.y, pixel, "StageEnd")) {
                m_CollisionPoint.emplace_back(point);
                stageEndCollisionDectected = true;
            }
        }

        UpdateCollisionState(bIsCollStage, stageCollisionDetected, "StageColl", &CMonster::OnStageCollisionEnter, &CMonster::OnStageCollisionExit);
        UpdateCollisionState(bIsCollWall, wallCollisionDetected, "Wall", &CMonster::OnWallCollisionEnter, &CMonster::OnWallCollisionExit);
        UpdateCollisionState(bIsCollStageEnd, stageEndCollisionDectected, "StageEnd", &CMonster::OnStageEndCollisionEnter, &CMonster::OnStageEndCollisionExit);
    }
    else {
        OutputDebugStringA("Pixel collider not set.\n");
    }
}

void CMonster::OnStageCollisionEnter() {
    CCollider tempCollider;
    tempCollider.SetColTag("StageColl");
    OnCollisionEnter(GetCollider().back(), &tempCollider);
}

void CMonster::OnStageCollisionExit() {
    CCollider tempCollider;
    tempCollider.SetColTag("StageColl");
    OnCollisionExit(GetCollider().back(), &tempCollider);
}

void CMonster::OnWallCollisionEnter() {
    CCollider tempCollider;
    tempCollider.SetColTag("Wall");
    OnWallCollisionEnter(GetCollider().back(), &tempCollider);
}

void CMonster::OnWallCollisionExit() {
    CCollider tempCollider;
    tempCollider.SetColTag("Wall");
    OnWallCollisionExit(GetCollider().back(), &tempCollider);
}

void CMonster::OnStageEndCollisionEnter() {
    CCollider tempCollider;
    tempCollider.SetColTag("StageEnd");
    OnStageEndCollisionEnter(GetCollider().back(), &tempCollider);
}

void CMonster::OnStageEndCollisionExit() {
    CCollider tempCollider;
    tempCollider.SetColTag("StageEnd");
    OnStageEndCollisionExit(GetCollider().back(), &tempCollider);
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
            GetAnimator()->Play(L"DeadLeft", true);
        }
        else
        {
            GetAnimator()->Play(L"DeadRight", true);
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
    CheckPixelColor();

    if (nullptr != m_pAI)
        m_pAI->Update();

    if (m_eCurMonState == MON_STATE::DEAD)
    {
        CheckDeathAnimation(); // �״� �ִϸ��̼� �Ϸ� ���� Ȯ��
        return; // �״� ������ ���� �ٸ� ������Ʈ ������ �������� ����
    }

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

    // �浹 ��ġ�� �ð�ȭ
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, hBrush);

    for (const Vec2& point : m_CollisionPoint) {
        Vec2 renderPos = CCamera::GetInst()->GetRenderPos(point);
        Ellipse(_dc, static_cast<int>(renderPos.x - 2), static_cast<int>(renderPos.y - 2), static_cast<int>(renderPos.x + 2), static_cast<int>(renderPos.y + 2));
    }

    SelectObject(_dc, hOldBrush);
    DeleteObject(hBrush);
}