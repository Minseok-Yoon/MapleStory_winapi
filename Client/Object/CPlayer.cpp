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
#include "../Scene/CScene.h"
#include "../Resource/CTexture.h"
#include "../Component/CCollider.h"
#include "../Component/CColliderPixel.h"
#include "../Component/CGravity.h"
#include "../Component/CAnimator.h"
#include "../Component/CRigidBody.h"

CPlayer::CPlayer() :
    m_eCurState(PLAYER_STATE::IDLE),
    m_ePrevState(PLAYER_STATE::IDLE),
    m_iDir(1),
    m_iPrevDir(1),
    m_bLeftEnable(true),
    m_bRightEnable(true),
    m_iWallCollisionCount(0),
    m_bIsColPortal(false),
    m_bIsColMonster(false),
    m_bStageCollision(false)
{
    AddCollider();
    auto playerCollider = GetCollider().back();
    playerCollider->SetColTag("Player");
    playerCollider->SetOffsetPos(Vec2{ 10.f, 0.f });
    playerCollider->SetScale(Vec2{ 20.f, 20.f });
    playerCollider->SetLayer(1);

    AddCollider();
    auto attackCollider = GetCollider().back();
    attackCollider->SetColTag("Attack");
    attackCollider->SetOffsetPos(Vec2{ 70.f, 0.f });
    attackCollider->SetScale(Vec2{ 20.f, 20.f });
    attackCollider->SetLayer(2);

    PlayerAnimationClip();

    CreateRigidBody();
    CreateGravity();
}

CPlayer::~CPlayer()
{
}

void CPlayer::PlayerAnimationClip()
{
    CreateAnimator();

    AddAnimationClip(L"StandRight", L"texture\\Player\\Idle\\Right\\%d.bmp", 3, 0.7f, 54.f, 65.f);
    AddAnimationClip(L"StandLeft", L"texture\\Player\\Idle\\Left\\%d.bmp", 3, 0.7f, 54.f, 65.f);
    AddAnimationClip(L"WalkRight", L"texture\\Player\\Walk\\Right\\%d.bmp", 4, 0.3f, 54.f, 65.f);
    AddAnimationClip(L"WalkLeft", L"texture\\Player\\Walk\\Left\\%d.bmp", 4, 0.3f, 54.f, 65.f);
    AddAnimationClip(L"JumpRight", L"texture\\Player\\Jump\\Right\\%d.bmp", 1, 0.3f, 54.f, 65.f);
    AddAnimationClip(L"JumpLeft", L"texture\\Player\\Jump\\Left\\%d.bmp", 1, 1.3f, 54.f, 65.f);
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
    if (m_bRopeCollision && (KEY_TAP(KEY::UP) || KEY_HOLD(KEY::UP)))
    {
        m_eCurState = PLAYER_STATE::ROPE;
    }

    if (KEY_TAP(KEY::LEFT) || KEY_HOLD(KEY::LEFT) || KEY_TAP(KEY::RIGHT) || KEY_HOLD(KEY::RIGHT))
    {
        m_eCurState = PLAYER_STATE::WALK;
    }
    else
    {
        m_eCurState = PLAYER_STATE::IDLE;
    }

    if (KEY_TAP(KEY::Z))
    {
        m_eCurState = PLAYER_STATE::JUMP;

        if (GetRigidBody())
        {
            GetRigidBody()->SetVelocity(Vec2(GetRigidBody()->GetVelocity().x, -200.f));
        }
    }
}

void CPlayer::Update_Move()
{
    Vec2 vPos = GetPos();

    if (m_eCurState == PLAYER_STATE::ROPE)
    {
        if (KEY_HOLD(KEY::UP))
        {
            vPos.y -= 200.f * fDeltaTime;
        }
        else if (KEY_HOLD(KEY::DOWN))
        {
            vPos.y += 200.f * fDeltaTime;
        }
    }

    if (KEY_HOLD(KEY::DOWN))
    {
        vPos.y += 300.f * fDeltaTime;
    }

    if (KEY_HOLD(KEY::LEFT) && m_bLeftEnable) // ���� �̵��� ������ ���
    {
        vPos.x -= 300.f * fDeltaTime;
        m_iDir = -1;
    }

    if (KEY_HOLD(KEY::RIGHT) && m_bRightEnable) // ������ �̵��� ������ ���
    {
        vPos.x += 300.f * fDeltaTime;
        m_iDir = 1;
    }

    // �� �浹 ���� Ȯ�� �� ��ġ ������Ʈ
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
        SetPos(vPos); // ���� �浹���� �ʴ� ��쿡�� ��ġ ������Ʈ
    }
}

void CPlayer::Update_Animation()
{
    switch (m_eCurState)
    {
    case PLAYER_STATE::IDLE:
    {
        if (m_iDir == -1)
        {
            GetAnimator()->Play(L"StandLeft", true);
        }
        else {
            GetAnimator()->Play(L"StandRight", true);
        }
    }
    break;

    case PLAYER_STATE::WALK:
    {
        if (m_iDir == -1)
        {
            GetAnimator()->Play(L"WalkLeft", true);
        }
        else {
            GetAnimator()->Play(L"WalkRight", true);
        }
    }
    break;

    case PLAYER_STATE::JUMP:
    {
        if (m_iDir == -1)
        {
            GetAnimator()->Play(L"JumpLeft", true);
        }
        else {
            GetAnimator()->Play(L"JumpRight", true);
        }
    }
    break;

    default:
        break;
    }

    m_ePrevState = m_eCurState;
    m_iPrevDir = m_iDir;
}

// �浹 ��ǥ ���
vector<Vec2> CPlayer::GetCollisionPoints(const Vec2& _vPos, int _iPlayerWidthHalf, int _iPlayerHeightHalf)
{
    vector<Vec2> collisionPoints;       // �浹 ��ǥ�� ����
    int offsetY = _iPlayerHeightHalf;

    // ���� ��ġ�� ���� ��ǥ�� ���
    int x = static_cast<int>(_vPos.x);
    int y = static_cast<int>(_vPos.y + offsetY);

    // �¿� �浹 �˻� ��ǥ�� ���
    int xLeft = x - _iPlayerWidthHalf;
    int xRight = x + _iPlayerWidthHalf;

    // ���� �浹 �˻� ��ǥ�� ���
    int yTop = y - _iPlayerHeightHalf;
    int yBottom = y + _iPlayerHeightHalf;

    // �浹��ǥ�� ���͸� �߰�
    collisionPoints.emplace_back(Vec2(x, y));
    collisionPoints.emplace_back(Vec2(xLeft, y));
    collisionPoints.emplace_back(Vec2(xRight, y));
    collisionPoints.emplace_back(Vec2(x, yTop));

    return collisionPoints;
}

// �ȼ� �浹 ���θ� Ȯ��
bool CPlayer::CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag)
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
        else if (_colTag == "Rope") {
            return (_pPixel.r == 0 && _pPixel.g == 128 && _pPixel.b == 0);
        }
    }
    else {
        OutputDebugStringA("Player position out of bounds.\n");
    }
    return false;
}

// �浹 ���¸� ������Ʈ
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
    // �������� �浹 �� �� �浹 ���¸� ��Ÿ���� ���� ����
    static bool isCollidingWithStage = false;
    static bool isCollidingWithWall = false;
    static bool isCollidingWithRope = false;

    // �ȼ� �浹 ��ü�� �����Ѵٸ�
    if (m_pPixelCollider) {
        Vec2 vPos = GetPos();

        int playerWidthHalf = 17;
        int playerHeightHalf = 32;
        int offsetY = playerHeightHalf;

        auto collisionPoints = GetCollisionPoints(vPos, playerWidthHalf, playerHeightHalf);
        m_CollisionPoint.clear(); // �浹 ��ġ ���� �ʱ�ȭ

        // �浹 ���� �÷��� �ʱ�ȭ
        bool stageCollisionDetected = false;
        bool wallCollisionDetected = false;
        bool ropeCollisionDetected = false;

        PIXEL pixel;

        // �������� �浹 �˻�
        const auto& stagePoint = collisionPoints[0];
        if (CheckPixelCollision(stagePoint.x, stagePoint.y, pixel, "StageColl")) {
            m_CollisionPoint.emplace_back(stagePoint);
            stageCollisionDetected = true;
        }

        // �� �浹 �˻�
        for (size_t i = 1; i <= 2; ++i) {
            const auto& point = collisionPoints[i];
            if (CheckPixelCollision(point.x, point.y, pixel, "Wall")) {
                m_CollisionPoint.emplace_back(point);
                wallCollisionDetected = true;
            }
        }

        // ���� �浹 �˻�
        const auto& ropePoint = collisionPoints.back();
        int ropeCollisionRadius = 10; // ���� �浹 ���� �ݰ�

        for (int dx = -ropeCollisionRadius; dx <= ropeCollisionRadius; dx += 1) { // X�� �������� ��ǥ �˻�
            if (CheckPixelCollision(ropePoint.x + dx, ropePoint.y, pixel, "Rope")) {
                m_CollisionPoint.emplace_back(Vec2(ropePoint.x + dx, ropePoint.y));
                ropeCollisionDetected = true;
                m_vRopePos = Vec2(ropePoint.x + dx, ropePoint.y); // ���� ��ġ ������Ʈ
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
        m_iWallCollisionCount++; // Wall �浹 Ƚ�� ����

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
        m_iWallCollisionCount++; // Wall �浹 Ƚ�� ����

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
        m_iWallCollisionCount--; // Wall �浹 Ƚ�� ����

        if (m_iWallCollisionCount <= 0) {
            m_iWallCollisionCount = 0; // �浹 Ƚ�� ���� ����

            // Wall �浹 ���°� ���� ���� ������ ���� ����
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
        GetGravity()->SetOnGround(false);
    }
}

void CPlayer::Update()
{
    CheckPixelColor();

    Update_State();
    Update_Move();
    Update_Animation();

    bool ropeCollisionDetected = m_bRopeCollision;
    bool spaceKeyTapDetected = KEY_TAP(KEY::SPACE);
    bool spaceKeyHoldDetected = KEY_HOLD(KEY::SPACE);

    if (m_bRopeCollision && spaceKeyTapDetected)
    {
        // ������ ��ġ�� �÷��̾� �̵�
        m_vPos.x = m_vRopePos.x;
        GetGravity()->SetOnGround(true);
    }

    else if(!m_bRopeCollision && !m_bStageCollision)
    {
        //GetGravity()->SetOnGround(false);
    }

    if (m_bStageCollision)
    {
        GetGravity()->SetOnGround(true);
    }

    if (m_bIsColPortal && KEY_TAP(KEY::UP))
    {
        // Portal�� �浹 ���̰� ���� ����Ű�� ���� ��� ChangeScene ȣ��
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

    wchar_t strState[100];
    swprintf_s(strState, L"Player State: %d", static_cast<int>(m_eCurState));
    TextOut(_dc, 200, 20, strState, lstrlen(strState));

    wchar_t strLeftEnable[100];
    swprintf_s(strLeftEnable, L"Left Enable: %s", m_bLeftEnable ? L"true" : L"false");
    TextOut(_dc, 200, 40, strLeftEnable, lstrlen(strLeftEnable));

    wchar_t strRightEnable[100];
    swprintf_s(strRightEnable, L"Right Enable: %s", m_bRightEnable ? L"true" : L"false");
    TextOut(_dc, 200, 60, strRightEnable, lstrlen(strRightEnable));

    // SetOnGround ���� ���
    wchar_t strOnGround[100];
    swprintf_s(strOnGround, L"On Ground: %s", GetGravity()->IsOnGround() ? L"true" : L"false");
    TextOut(_dc, 200, 80, strOnGround, lstrlen(strOnGround));

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