#include "../pch.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CObject.h"
#include "CPortal.h"
#include "../Manager/CColliderManager.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CDamageManager.h"
#include "../Scene/CScene.h"
#include "../Resource/CTexture.h"
#include "../Component/CCollider.h"
#include "../Component/CColliderPixel.h"
#include "../Component/CGravity.h"
#include "../Component/CAnimator.h"
#include "../Component/CAnimation.h"
#include "../Component/CRigidBody.h"

CPlayer::CPlayer() :
    m_eCurState(PLAYER_STATE::IDLE),
    m_ePrevState(PLAYER_STATE::IDLE),
    m_iDir(1),
    m_iPrevDir(1),
    m_bLeftEnable(true),
    m_bRightEnable(true),
    m_iWallCollisionCount(0),
    m_bJumpCycle(false),
    m_bAttackCycle(false),
    m_bIsColPortal(false),
    m_bIsColMonster(false),
    m_bStageCollision(false),
    m_fAttackDelayTime(1.2f),  // 공격 후 0.2초 동안 대기
    m_fElapsedTime(0.0f)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    InitColliders();
    PlayerAnimationClip();
    CreateRigidBody();
    CreateGravity();
}

CPlayer::~CPlayer()
{
}

void CPlayer::InitColliders()
{
    AddCollider();
    auto playerCollider = GetCollider().back();
    playerCollider->SetColTag("Player");
    playerCollider->SetOffsetPos(Vec2{ 0.f, 0.f });
    playerCollider->SetScale(Vec2{ 20.f, 20.f });
    playerCollider->SetLayer(1);

    AddCollider();
    m_pRightAttackCollider = GetCollider().back();
    m_pRightAttackCollider->SetColTag("Right_Attack");
    m_pRightAttackCollider->SetOffsetPos(Vec2{ 50.f, 0.f });
    m_pRightAttackCollider->SetScale(Vec2{ 40.f, 80.f });
    m_pRightAttackCollider->SetLayer(2);

    AddCollider();
    m_pLeftAttackCollider = GetCollider().back();
    m_pLeftAttackCollider->SetColTag("Left_Attack");
    m_pLeftAttackCollider->SetOffsetPos(Vec2{ -50.f, 0.f });
    m_pLeftAttackCollider->SetScale(Vec2{ 40.f, 80.f });
    m_pLeftAttackCollider->SetLayer(3);
}

void CPlayer::PlayerAnimationClip()
{
    CreateAnimator();

    AddAnimationClip(L"StandRight", L"texture\\Player\\Idle\\Right\\%d.bmp", 3, 0.7f, 54.f, 65.f);
    AddAnimationClip(L"StandLeft", L"texture\\Player\\Idle\\Left\\%d.bmp", 3, 0.7f, 54.f, 65.f);
    AddAnimationClip(L"WalkRight", L"texture\\Player\\Walk\\Right\\%d.bmp", 4, 0.3f, 54.f, 65.f);
    AddAnimationClip(L"WalkLeft", L"texture\\Player\\Walk\\Left\\%d.bmp", 4, 0.3f, 54.f, 65.f);
    AddAnimationClip(L"JumpRight", L"texture\\Player\\Jump\\Right\\%d.bmp", 1, 1.3f, 54.f, 65.f);
    AddAnimationClip(L"JumpLeft", L"texture\\Player\\Jump\\Left\\%d.bmp", 1, 1.3f, 54.f, 65.f);
    AddAnimationClip(L"Rope", L"texture\\Player\\Rope\\%d.bmp", 2, 0.9f, 54.f, 65.f);
    AddAnimationClip(L"AttackRight", L"texture\\Player\\Attack\\Right\\%d.bmp", 3, 0.2f, 55.f, 65.f);
    AddAnimationClip(L"AttackLeft", L"texture\\Player\\Attack\\Left\\%d.bmp", 3, 0.2f, 55.f, 65.f);
}

void CPlayer::AddAnimationClip(const wstring& _strKey, const wchar_t* _pFilePath, int _iFrameMax, float _fAnimationLimitTime, float _fFrameSizeX, float _fFrameSizeY)
{
    vector<wstring> vecFile;

    for (int i = 1; i <= _iFrameMax; i++)
    {
        wchar_t strFileName[MAX_PATH] = {};
        wsprintf(strFileName, _pFilePath, i);
        vecFile.push_back(strFileName);
    }

    GetAnimator()->CreateFrameAnimation(_strKey, vecFile, Vec2(0.f, 0.f), Vec2(_fFrameSizeX, _fFrameSizeY), _fAnimationLimitTime);
}

void CPlayer::Update_State()
{
    // 공격 입력 처리
    if (KEY_TAP(KEY::X))
    {
        if (m_eCurState != PLAYER_STATE::ATTACK) // 이전 상태와 동일하지 않은 경우만 변경
        {
            m_eCurState = PLAYER_STATE::ATTACK;
            m_bAttackCycle = true;
            m_fElapsedTime = 0.0f;  // 경과 시간 초기화
            Update_Animation();  // 공격 애니메이션 시작

            // 방향에 따른 충돌체 활성화/비활성화
            if (m_iDir == -1) // 왼쪽을 보고 있을 때
            {
                m_pLeftAttackCollider->Enable(true);   // 왼쪽 충돌체 활성화
                m_pRightAttackCollider->Enable(false); // 오른쪽 충돌체 비활성화
                GetAnimator()->FindAnimation(L"AttackLeft")->SetFrame(0);
            }
            else // 오른쪽을 보고 있을 때
            {
                m_pLeftAttackCollider->Enable(false);  // 왼쪽 충돌체 비활성화
                m_pRightAttackCollider->Enable(true);  // 오른쪽 충돌체 활성화
                GetAnimator()->FindAnimation(L"AttackRight")->SetFrame(0);
            }

            return;
        }
        return;
    }

    // 공격 상태 처리
    if (m_eCurState == PLAYER_STATE::ATTACK)
    {
        m_fElapsedTime += fDeltaTime;  // 경과 시간 누적

        // 애니메이션이 끝나고 딜레이가 지난 경우에만 상태를 IDLE로 전환
        if (GetAnimator()->End() && m_fElapsedTime >= m_fAttackDelayTime)
        {
            m_bAttackCycle = false;  // 공격 사이클 종료
            m_pRightAttackCollider->Enable(false); // 오른쪽 충돌체 비활성화
            m_pLeftAttackCollider->Enable(false);  // 왼쪽 충돌체 비활성화

            // 충돌한 몬스터가 있는지 확인하고, 공격이 성공했는지 확인
            if (m_pTargetMonster != nullptr)
            {
                PlayerAttack(m_pTargetMonster);  // 공격 처리
                m_pTargetMonster = nullptr;  // 처리 후 타겟 초기화
            }

            m_eCurState = PLAYER_STATE::IDLE;  // 상태를 IDLE로 전환
            Update_Animation();  // 애니메이션을 IDLE로 갱신
        }
        return; // 공격 상태에서는 다른 상태로 전환하지 않음
    }

    // 점프 상태 처리
    if (m_eCurState == PLAYER_STATE::JUMP)
    {
        m_bJumpCycle = true;
        if (GetGravity()->IsOnGround())
        {
            m_bJumpCycle = false;
            m_eCurState = PLAYER_STATE::IDLE;
            Update_Animation();  // 애니메이션 갱신
        }
        return;
    }

    // 점프 입력 처리
    if (KEY_TAP(KEY::Z) || KEY_HOLD(KEY::Z))
    {
        m_eCurState = PLAYER_STATE::JUMP;
        m_bJumpCycle = true;
        GetGravity()->Jump(300.f);
        return;
    }

    // 로프 상태 처리
    if ((KEY_TAP(KEY::UP) && m_bRopeCollision) ||
        (KEY_HOLD(KEY::UP) && m_bRopeCollision) ||
        (KEY_TAP(KEY::DOWN) && m_bRopeCollision) ||
        (KEY_HOLD(KEY::DOWN) && m_bRopeCollision))
    {
        m_eCurState = PLAYER_STATE::ROPE;
        m_bRopeCollision = true;
        return;
    }

    // 걷기 상태 처리
    if (KEY_TAP(KEY::LEFT) || KEY_HOLD(KEY::LEFT) || KEY_TAP(KEY::RIGHT) || KEY_HOLD(KEY::RIGHT))
    {
        m_eCurState = PLAYER_STATE::WALK;
        return;
    }

    // 기본 상태(IDLE)로 전환
    if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::ATTACK && m_eCurState != PLAYER_STATE::ROPE)
    {
        m_bJumpCycle = false;
        m_bAttackCycle = false;
        m_bRopeCycle = false;
        m_eCurState = PLAYER_STATE::IDLE;
        Update_Animation();  // 애니메이션 갱신
    }
}

void CPlayer::Update_Move()
{
    Vec2 vPos = GetPos();

    if (m_eCurState == PLAYER_STATE::ROPE)
    {
        if (KEY_HOLD(KEY::UP))
        {
            vPos.y -= 100.f * fDeltaTime;
        }
        else if (KEY_HOLD(KEY::DOWN))
        {
            vPos.y += 100.f * fDeltaTime;
        }
    }

    if (KEY_HOLD(KEY::LEFT) && m_bLeftEnable) // 왼쪽 이동이 가능한 경우
    {
        vPos.x -= 300.f * fDeltaTime;
        m_iDir = -1;
    }

    if (KEY_HOLD(KEY::RIGHT) && m_bRightEnable) // 오른쪽 이동이 가능한 경우
    {
        vPos.x += 300.f * fDeltaTime;
        m_iDir = 1;
    }

    // 벽 충돌 상태 확인 및 위치 업데이트
    bool isCollidingWithWall = false;
    if (m_iDir == -1 && !m_bLeftEnable)
    {
        isCollidingWithWall = true;
    }
    else if (m_iDir == 1 && !m_bRightEnable)
    {
        isCollidingWithWall = true;
    }

    if (!isCollidingWithWall)
    {
        SetPos(vPos); // 벽과 충돌하지 않는 경우에만 위치 업데이트
    }
}

void CPlayer::Update_Animation()
{
    switch (m_eCurState)
    {
    case PLAYER_STATE::IDLE:
        if (m_iDir == -1)
        {
            GetAnimator()->Play(L"StandLeft", true);
        }
        else
        {
            GetAnimator()->Play(L"StandRight", true);
        }
        break;

    case PLAYER_STATE::WALK:
        if (m_iDir == -1)
        {
            GetAnimator()->Play(L"WalkLeft", true);
        }
        else
        {
            GetAnimator()->Play(L"WalkRight", true);
        }
        break;

    case PLAYER_STATE::JUMP:
        if (m_iDir == -1)
        {
            GetAnimator()->Play(L"JumpLeft", true);
        }
        else
        {
            GetAnimator()->Play(L"JumpRight", true);
        }
        break;

    case PLAYER_STATE::ROPE:
        GetAnimator()->Play(L"Rope", true);
        break;

    case PLAYER_STATE::ATTACK:
        if (m_iDir == -1)
        {
            GetAnimator()->Play(L"AttackLeft", false);
        }
        else
        {
            GetAnimator()->Play(L"AttackRight", false);
        }
        break;

    default:
        break;
    }

    m_ePrevState = m_eCurState;
    m_iPrevDir = m_iDir;
}

vector<Vec2> CPlayer::GetCollisionPoints(const Vec2& _vPos, int _iPlayerWidthHalf, int _iPlayerHeightHalf)
{
    vector<Vec2> collisionPoints;       // 충돌 좌표를 저장
    int offsetY = _iPlayerHeightHalf;

    // 현재 위치의 정수 좌표로 계산
    int x = static_cast<int>(_vPos.x);
    int y = static_cast<int>(_vPos.y + offsetY);

    // 좌우 충돌 검사 좌표를 계산
    int xLeft = x - _iPlayerWidthHalf;
    int xRight = x + _iPlayerWidthHalf;

    // 상하 충돌 검사 좌표를 계산
    int yTop = y - 10.f;
    int yBottom = y + _iPlayerHeightHalf;

    // 충돌좌표에 벡터를 추가
    collisionPoints.emplace_back(Vec2(x, y));
    collisionPoints.emplace_back(Vec2(xLeft, y));
    collisionPoints.emplace_back(Vec2(xRight, y));
    collisionPoints.emplace_back(Vec2(x, yTop));

    return collisionPoints;
}

bool CPlayer::CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag)
{
    // 좌표가 픽셀 충돌 객체의 범위 내에 있는지 확인
    if (_iPosX >= 0 && _iPosX < m_pPixelCollider->GetWidth() && _iPosY >= 0 && _iPosY < m_pPixelCollider->GetHeight()) {
        _pPixel = m_pPixelCollider->GetPixelColor(_iPosX, _iPosY);

        // 충돌 태그에 따라 충돌 여부를 판정
        if (_colTag == "StageColl") {
            return (_pPixel.r == 255 && _pPixel.g == 0 && _pPixel.b == 255);
        }
        else if (_colTag == "Wall") {
            return (_pPixel.r == 0 && _pPixel.g == 255 && _pPixel.b == 255);
        }
        else if (_colTag == "Rope") {
            return (_pPixel.r == 0 && _pPixel.g == 128 && _pPixel.b == 0);
        }
    }
    else {
        OutputDebugStringA("Player position out of bounds.\n");
    }
    return false;
}

// 충돌 상태를 업데이트
void CPlayer::UpdateCollisionState(bool& _bIsColiding, bool _bCollisionDetected, const string& _strColTag, void(CPlayer::* onEnter)(), void(CPlayer::* onExit)())
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

void CPlayer::CheckPixelColor()
{
    // 스테이지 충돌 및 벽, 로프 충돌 상태를 나타내는 정적 변수
    static bool isCollidingWithStage = false;
    static bool isCollidingWithWall = false;
    static bool isCollidingWithRope = false;

    // 픽셀 충돌 객체가 존재한다면
    if (m_pPixelCollider) {
        Vec2 vPos = GetPos();

        int playerWidthHalf = 17;
        int playerHeightHalf = 32;

        auto collisionPoints = GetCollisionPoints(vPos, playerWidthHalf, playerHeightHalf);
        m_CollisionPoint.clear(); // 충돌 위치 벡터 초기화

        // 충돌 감지 플래그 초기화
        bool stageCollisionDetected = false;
        bool wallCollisionDetected = false;
        bool ropeCollisionDetected = false;

        PIXEL pixel;

        // 스테이지 충돌 검사
        const auto& stagePoint = collisionPoints[0];
        if (CheckPixelCollision(stagePoint.x, stagePoint.y, pixel, "StageColl")) {
            m_CollisionPoint.emplace_back(stagePoint);
            stageCollisionDetected = true;
        }

        // 벽 충돌 검사
        for (size_t i = 1; i <= 2; ++i) {
            const auto& point = collisionPoints[i];
            if (CheckPixelCollision(point.x, point.y, pixel, "Wall")) {
                m_CollisionPoint.emplace_back(point);
                wallCollisionDetected = true;
            }
        }

        // 로프 충돌 검사
        const auto& ropePoint = collisionPoints.back();
        int ropeCollisionRadius = 10; // 로프 충돌 범위 반경

        for (int dx = -ropeCollisionRadius; dx <= ropeCollisionRadius; dx += 1) { // X축 방향으로 좌표 검사
            if (CheckPixelCollision(ropePoint.x + dx, ropePoint.y, pixel, "Rope")) {
                m_CollisionPoint.emplace_back(Vec2(ropePoint.x + dx, ropePoint.y));
                ropeCollisionDetected = true;
                m_vRopePos = Vec2(ropePoint.x + dx, ropePoint.y); // 로프 위치 업데이트
            }
        }

        UpdateCollisionState(isCollidingWithStage, stageCollisionDetected, "StageColl", &CPlayer::OnStageCollisionEnter, &CPlayer::OnStageCollisionExit);
        UpdateCollisionState(isCollidingWithWall, wallCollisionDetected, "Wall", &CPlayer::OnWallCollisionEnter, &CPlayer::OnWallCollisionExit);
        UpdateCollisionState(isCollidingWithRope, ropeCollisionDetected, "Rope", &CPlayer::OnRopeCollisionEnter, &CPlayer::OnRopeCollisionExit);
    }
    else {
        OutputDebugStringA("Pixel collider not set.\n");
    }
}

void CPlayer::OnStageCollisionEnter() {
    CCollider tempCollider;
    tempCollider.SetColTag("StageColl");
    OnStageCollisionEnter(&tempCollider);
}

void CPlayer::OnStageCollisionExit() {
    CCollider tempCollider;
    tempCollider.SetColTag("StageColl");
    OnStageCollisionExit(&tempCollider);
}

void CPlayer::OnWallCollisionEnter() {
    CCollider tempCollider;
    tempCollider.SetColTag("Wall");
    OnWallCollisionEnter(&tempCollider);
}

void CPlayer::OnWallCollisionExit() {
    CCollider tempCollider;
    tempCollider.SetColTag("Wall");
    OnWallCollisionExit(&tempCollider);
}

void CPlayer::OnRopeCollisionEnter()
{
    CCollider tempCollider;
    tempCollider.SetColTag("Rope");
    OnRopeCollisionEnter(&tempCollider);
}

void CPlayer::OnRopeCollisionExit()
{
    CCollider tempCollider;
    tempCollider.SetColTag("Rope");
    OnRopeCollisionExit(&tempCollider);
}

void CPlayer::OnCollisionEnter(CCollider* _ColTag, CCollider* _pOther)
{
    if (_ColTag->GetColTag() == "Player" && _pOther->GetColTag() == "Portal 0")
    {
        m_bIsColPortal = true;
    }

    if (_pOther->GetColTag() == "Player" && _pOther->GetColTag() == "Monster")
    {
        m_bIsColMonster = true;
    }

    if (_ColTag->GetColTag() == "Player" && _pOther->GetColTag() == "Item")
    {
    }
}

void CPlayer::OnCollision(CCollider* _ColTag, CCollider* _pOther)
{
    if (_ColTag->GetColTag() == "Right_Attack" || _ColTag->GetColTag() == "Left_Attack")
    {
        if (_pOther->GetColTag() == "Monster")
        {
            m_pTargetMonster = dynamic_cast<CMonster*>(_pOther->GetOwner()); // 충돌한 몬스터를 타겟으로 설정
        }
    }
}

void CPlayer::OnCollisionExit(CCollider* _ColTag, CCollider* _pOther)
{
    if (_pOther->GetColTag() == "Portal 0")
    {
        m_bIsColPortal = false;
        m_strPortalTag.clear();
    }

    if (_ColTag->GetColTag() == "Player" && _pOther->GetColTag() == "Monster")
    {
        m_bIsColMonster = false;
    }
}

void CPlayer::OnWallCollisionEnter(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "Wall")
    {
        m_iWallCollisionCount++; // Wall 충돌 횟수 증가

        if (m_iDir == -1) {
            m_bLeftEnable = false;
        }
        else if (m_iDir == 1)
        {
            m_bRightEnable = false;
        }
    }
}

void CPlayer::OnWallCollision(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "Wall")
    {
        m_iWallCollisionCount++; // Wall 충돌 횟수 증가

        if (m_iDir == -1) {
            m_bLeftEnable = false;
        }
        else if (m_iDir == 1)
        {
            m_bRightEnable = false;
        }
    }
}

void CPlayer::OnWallCollisionExit(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "Wall")
    {
        m_iWallCollisionCount--; // Wall 충돌 횟수 감소

        if (m_iWallCollisionCount <= 0) {
            m_iWallCollisionCount = 0; // 충돌 횟수 음수 방지

            // Wall 충돌 상태가 없을 때만 움직임 제어 해제
            m_bLeftEnable = true;
            m_bRightEnable = true;
        }
    }
}

void CPlayer::OnStageCollisionEnter(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageColl")
    {
        GetGravity()->SetOnGround(true);
        Vec2 vPos = GetPos();
        vPos.y = floor(vPos.y);
        SetPos(vPos);
    }
}

void CPlayer::OnStageCollision(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageColl")
    {
        m_bStageCollision = true;
        GetGravity()->SetOnGround(true);
        Vec2 vPos = GetPos();
        vPos.y = floor(vPos.y);
        SetPos(vPos);
    }
}

void CPlayer::OnStageCollisionExit(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageColl")
    {
        GetGravity()->SetOnGround(false);
    }
}

void CPlayer::OnRopeCollisionEnter(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "Rope")
    {
        m_bRopeCollision = true;
    }
}

void CPlayer::OnRopeCollision(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "Rope")
    {
        m_bRopeCollision = true;
    }
}

void CPlayer::OnRopeCollisionExit(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "Rope")
    {
        m_bRopeCollision = false;

        if (m_eCurState == PLAYER_STATE::ROPE)
        {
            GetGravity()->SetOnGround(false);
        }
    }
}

void CPlayer::PlayerAttack(CMonster* monster)
{
    if (monster)
    {
        bool isCriticalHit = (rand() % 10) < 2;  // 20% 확률로 크리티컬 히트

        int attackDamage;
        if (isCriticalHit)
        {
            // 크리티컬 데미지: 30 ~ 200
            attackDamage = 30 + rand() % 171;  // rand() % 171은 0 ~ 170 범위의 값 생성
        }
        else
        {
            // 일반 데미지: 0 ~ 30
            attackDamage = rand() % 31;  // rand() % 31은 0 ~ 30 범위의 값 생성
        }

        // 몬스터 위치에서 데미지를 출력
        Vec2 monsterPos = monster->GetPos();
        CDamageManager::GetInst()->CreateDamage(attackDamage, monsterPos, isCriticalHit);

        // 몬스터에 데미지를 입힘
        monster->ReduceHP(attackDamage);
    }
}

void CPlayer::Update()
{
    CheckPixelColor();

    // 경과 시간 갱신
    m_fElapsedTime += fDeltaTime;

    Update_State();
    Update_Animation();
    Update_Move();

    bool ropeCollisionDetected = m_bRopeCollision;
    bool spaceKeyTapDetected = KEY_TAP(KEY::SPACE);
    bool spaceKeyHoldDetected = KEY_HOLD(KEY::SPACE);

    if (m_bRopeCollision && spaceKeyTapDetected)
    {
        // 로프의 위치로 플레이어 이동
        m_vPos.x = m_vRopePos.x;
        m_eCurState = PLAYER_STATE::ROPE;
        GetGravity()->SetOnGround(true);
    }

    if (m_bStageCollision)
    {
        GetGravity()->SetOnGround(true);
    }

    if (m_bIsColPortal && KEY_TAP(KEY::UP))
    {
        // Portal과 충돌 중이고 위쪽 방향키가 눌린 경우 ChangeScene 호출
        ChangeScene(SCENE_TYPE::STAGE_02);
    }

    m_ePrevState = m_eCurState;
    m_iPrevDir = m_iDir;
}

void CPlayer::Render(HDC _dc)
{
    CObject::ComponentRender(_dc);

    Vec2 vPos = GetPos();
    vPos = CCamera::GetInst()->GetRenderPos(vPos);

    //wchar_t strState[100];
    //swprintf_s(strState, L"Player State: %d", static_cast<int>(m_eCurState));
    //TextOut(_dc, 200, 20, strState, lstrlen(strState));

    //wchar_t strLeftEnable[100];
    //swprintf_s(strLeftEnable, L"Left Enable: %s", m_bLeftEnable ? L"true" : L"false");
    //TextOut(_dc, 200, 40, strLeftEnable, lstrlen(strLeftEnable));

    //wchar_t strRightEnable[100];
    //swprintf_s(strRightEnable, L"Right Enable: %s", m_bRightEnable ? L"true" : L"false");
    //TextOut(_dc, 200, 60, strRightEnable, lstrlen(strRightEnable));

    //wchar_t strRopeEnable[100];
    //swprintf_s(strRopeEnable, L"Rope Enable: %s", m_bRopeCollision ? L"true" : L"false");
    //TextOut(_dc, 200, 100, strRopeEnable, lstrlen(strRopeEnable));

    //// SetOnGround 상태 출력
    //wchar_t strOnGround[100];
    //swprintf_s(strOnGround, L"On Ground: %s", GetGravity()->IsOnGround() ? L"true" : L"false");
    //TextOut(_dc, 200, 80, strOnGround, lstrlen(strOnGround));

    //wchar_t strJumpCycle[100];
    //swprintf_s(strJumpCycle, L"JumpCycle: %s", m_bJumpCycle ? L"true" : L"false");
    //TextOut(_dc, 200, 120, strJumpCycle, lstrlen(strJumpCycle));

    //wchar_t strAttackCycle[100];
    //swprintf_s(strAttackCycle, L"AttackCycle: %s", m_bAttackCycle ? L"true" : L"false");
    //TextOut(_dc, 200, 140, strAttackCycle, lstrlen(strAttackCycle));

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