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
#include "../Component/CRigidBody.h"

CMonster::CMonster()
    : m_tMonInfo(),
    m_fSpeed(100.f), // 초기 속도 설정
    m_iDir(1), // 초기 이동 방향 설정 (1: 오른쪽, -1: 왼쪽)
    m_vCenterPos(Vec2(300.f, 0.f)), // 초기 중심 위치
    m_fMaxDistance(200.f), // 초기 최대 배회 거리
    m_eCurMonState(MON_STATE::PATROL), // 초기 상태를 PATROL로 설정
    m_ePrevMonState(MON_STATE::IDLE),
    m_fIdleTime(0.f), // 대기 시간 초기화
    m_bWaiting(false) // 대기 상태 초기화
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
    CheckPixelColor();

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

void CMonster::DropItem()
{
    CScene* pScene = CSceneManager::GetInst()->GetCurScene();
    Vec2 vPos = GetPos();

    if (pScene)
    {
        // 50% 확률로 아이템 드롭
        int itemDropChance = std::rand() % 100; // 0-99 사이의 랜덤 숫자 생성
        if (itemDropChance < 100)
        {
            CItem* pItem = new CItem();
            Vec2 vItemOffset = Vec2(-10.f, 0.f); // 아이템의 오프셋 설정 (왼쪽으로 10단위)
            Vec2 vItemPos = vPos + vItemOffset;
            pItem->SetPos(vItemPos); // 몬스터의 위치에서 오프셋을 더하여 설정
            pScene->AddObject(pItem, OBJECT_TYPE::ITEM);
            OutputDebugStringA("Item dropped!\n");
        }

        // 100% 확률로 돈 드롭
        CMeso* pMoney = new CMeso();
        Vec2 vMoneyOffset = Vec2(20.f, 0.f); // 돈의 오프셋 설정 (오른쪽으로 20단위)
        Vec2 vMoneyPos = vPos + vMoneyOffset;
        pMoney->SetPos(vMoneyPos); // 몬스터의 위치에서 오프셋을 더하여 설정
        pMoney->SetMoneyAmount(100);
        pScene->AddObject(pMoney, OBJECT_TYPE::MESO);
        OutputDebugStringA("Money dropped!\n");
    }
    else
    {
        OutputDebugStringA("Failed to drop item or money: Scene not set.\n");
    }
}

void CMonster::OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther)
{
    std::string colTag = _ColTag->GetColTag();
    OutputDebugStringA(("OnCollisionEnter called with tag: " + colTag + "\n").c_str());

    if (_pOther->GetColTag() == "StageColl")
    {
        OutputDebugStringA("Collision Enter with StageColl detected.\n");
        Vec2 vPos = GetPos();
        vPos.y = floor(vPos.y);
        SetPos(vPos);
        GetGravity()->SetOnGround(true);
    }

    if (_pOther->GetColTag() == "Attack")
    {
        m_iHP -= 1;
        if (m_iHP <= 0 && m_eCurMonState != MON_STATE::DEAD)
        {
            m_eCurMonState = MON_STATE::DEAD;
            Sleep(3.5);
            DropItem();
            DeleteObject(this);
        }
    }
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

void CMonster::OnStageEndCollision(CCollider* _ColTag, CCollider* _pOther)
{
}

void CMonster::OnStageEndCollisionEnter(CCollider* _ColTag, CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageCollEnd")
    {
        if (m_eCurMonState == MON_STATE::PATROL || m_eCurMonState == MON_STATE::TRACE) {
            m_iDir *= -1; // 방향 전환
        }
    }
}

void CMonster::OnStageEndCollisionExit(CCollider* _ColTag, CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageCollEnd")
    {
    }
}

vector<Vec2> CMonster::GetCollisionPoint(const Vec2& _vPos, int _iMonHeightHalf)
{
    vector<Vec2> collisionPoint;
    int offsetY = _iMonHeightHalf;

    // 현재 위치의 정수 좌표로 계산
    int x = static_cast<int>(_vPos.x);
    int y = static_cast<int>(_vPos.y + offsetY);

    // 좌우 충돌 검사 좌표를 계산
    int xLeft = x - 10.f;
    int xRight = x + 10.f;

    // 충돌 좌표에  벡터를 추가
    collisionPoint.emplace_back(Vec2(x, y));
    collisionPoint.emplace_back(Vec2(xLeft, y));
    collisionPoint.emplace_back(Vec2(xRight, y));

    return collisionPoint;
}

bool CMonster::CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag)
{
    // 좌표가 픽셀 충돌 객체의 범위 내에 있는지 확인
    if (_iPosX >= 0 && _iPosX < m_pPixelCollider->GetWidth() && _iPosY >= 0 && _iPosY < m_pPixelCollider->GetHeight()) {
        _pPixel = m_pPixelCollider->GetPixelColor(_iPosX, _iPosY);

        // 픽셀 값 디버깅 출력
        char debugOutput[100];
        snprintf(debugOutput, sizeof(debugOutput), "Monster Pixel Color at (%d, %d): (%d, %d, %d)\n",
            _iPosX, _iPosY, static_cast<int>(_pPixel.r), static_cast<int>(_pPixel.g), static_cast<int>(_pPixel.b));
        OutputDebugStringA(debugOutput);

        // 충돌 태그에 따라 충돌 여부를 판정
        if (_colTag == "StageColl") {
            return (_pPixel.r == 255 && _pPixel.g == 0 && _pPixel.b == 255);
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

        int monHeightHalf = 29; // 예: 플레이어 높이의 절반
        int offsetY = monHeightHalf;

        auto collisionPoints = GetCollisionPoint(vPos, monHeightHalf);
        m_CollisionPoint.clear();

        // 충돌 감지 플래그 초기화
        bool stageCollisionDetected = false;
        bool stageEndCollisionDectected = false;

        PIXEL pixel;
        for (const auto& point : collisionPoints) {
            if (CheckPixelCollision(point.x, point.y, pixel, "StageColl")) {
                m_CollisionPoint.emplace_back(point);
                stageCollisionDetected = true;
            }
        }

        // 벽 충돌 검사
        for (size_t i = 1; i <= 2; ++i) {
            const auto& point = collisionPoints[i];
            if (CheckPixelCollision(point.x, point.y, pixel, "Wall")) {
                m_CollisionPoint.emplace_back(point);
                stageEndCollisionDectected = true;
            }
        }

        UpdateCollisionState(bIsCollStage, stageCollisionDetected, "StageColl", &CMonster::OnStageCollisionEnter, &CMonster::OnStageCollisionExit);
        UpdateCollisionState(bIsCollStageEnd, stageCollisionDetected, "StageCollEnd", &CMonster::OnStageEndCollisionEnter, &CMonster::OnStageEndCollisionExit);
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

void CMonster::OnStageEndCollisionEnter() {
    CCollider tempCollider;
    tempCollider.SetColTag("StageCollEnd");
    OnCollisionEnter(GetCollider().back(), &tempCollider);
}

void CMonster::OnStageEndCollisionExit() {
    CCollider tempCollider;
    tempCollider.SetColTag("StageCollEnd");
    OnCollisionExit(GetCollider().back(), &tempCollider);
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
    CheckPixelColor();

    if (nullptr != m_pAI)
        m_pAI->Update();

    Vec2 vMonCurPos = GetPos();

    if (m_eCurMonState == MON_STATE::PATROL)
    {
        // 진행 방향으로 초당 speed만큼 이동
        vMonCurPos.x += m_fSpeed * m_iDir * fDeltaTime;

        // 배회거리 기준량을 넘어섰는지 확인
        float fDist = abs(m_vCenterPos.x - vMonCurPos.x) - m_fMaxDistance;

        if (0.f < fDist)
        {
            // 배회 거리를 초과한 경우, 대기 상태로 전환
            //m_iDir *= -1;
            m_fIdleTime = 4.f; // 4초 대기 시간 설정
            m_bWaiting = true;
            m_eCurMonState = MON_STATE::IDLE;
        }
    }
    else if (m_eCurMonState == MON_STATE::IDLE)
    {
        // 대기 상태일 때 타이머를 감소시킴
        if (m_bWaiting)
        {
            m_fIdleTime -= fDeltaTime;

            if (m_fIdleTime <= 0.f)
            {
                // 대기 시간이 끝나면 다시 배회 상태로 전환
                m_bWaiting = false;
                m_eCurMonState = MON_STATE::PATROL;
                // 방향 전환 후 바로 이동하지 않도록 vMonCurPos 업데이트
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

    // 충돌 위치를 시각화
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, hBrush);

    for (const Vec2& point : m_CollisionPoint) {
        Vec2 renderPos = CCamera::GetInst()->GetRenderPos(point);
        Ellipse(_dc, static_cast<int>(renderPos.x - 2), static_cast<int>(renderPos.y - 2), static_cast<int>(renderPos.x + 2), static_cast<int>(renderPos.y + 2));
    }

    SelectObject(_dc, hOldBrush);
    DeleteObject(hBrush);
}