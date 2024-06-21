#include "../pch.h"
#include "CPlayer.h"
#include "../Object/CObject.h"
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

CPlayer::CPlayer() :
    m_eCurState(PLAYER_STATE::IDLE),
    m_ePrevState(PLAYER_STATE::IDLE),
    m_iDir(1),
    m_iPrevDir(1),
    m_bLeftEnable(true),
    m_bRightEnable(true),
    m_bIsOnWall(false)
{
    CreateCollider();
    GetCollider()->SetOffsetPos(Vec2{ 0.f, 0.f });
    GetCollider()->SetScale(Vec2{ 20.f, 20.f });

    PlayerAnimationClip(); // Initialize animations

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

void CPlayer::AddAnimationClip(const wstring& strKey, const wchar_t* pFilePath, int iFrameMax, float fAnimationLimitTime, float fFrameSizeX, float fFrameSizeY)
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

void CPlayer::Update_State()
{
    if (KEY_TAP(KEY::A) || KEY_HOLD(KEY::A) || KEY_TAP(KEY::D) || KEY_HOLD(KEY::D))
    {
        m_eCurState = PLAYER_STATE::WALK;
    }
    else if (KEY_HOLD(KEY::W) || KEY_TAP(KEY::W))
    {
        m_eCurState = PLAYER_STATE::JUMP;
    }
    else
    {
        m_eCurState = PLAYER_STATE::IDLE; // ����: IDLE ���·� ����
    }
}

void CPlayer::Update_Move()
{
    Vec2 vPos = GetPos();

    if (KEY_HOLD(KEY::W))
    {
        vPos.y -= 500.f * fDeltaTime;
    }

    if (KEY_HOLD(KEY::S))
    {
        vPos.y += 500.f * fDeltaTime;
    }

    if (KEY_HOLD(KEY::A) && m_bLeftEnable) // ���� �̵��� ������ ���
    {
        vPos.x -= 500.f * fDeltaTime;
        m_iDir = -1;
    }

    if (KEY_HOLD(KEY::D) && m_bRightEnable) // ������ �̵��� ������ ���
    {
        vPos.x += 500.f * fDeltaTime;
        m_iDir = 1;
    }

    SetPos(vPos);
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
        OutputDebugStringA("Current State is not IDLE\n");
        break;
    }

    m_ePrevState = m_eCurState;
    m_iPrevDir = m_iDir;
}

void CPlayer::CheckPixelColor()
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

            if (pixel.r == 255 && pixel.g == 0 && pixel.b == 255) {
                if (GetCollider() && GetCollider()->GetColTag() != "StageColl") {
                    GetCollider()->SetColTag("StageColl");

                    // ���� CCollider ��ü ����
                    CCollider tempCollider;
                    tempCollider.SetColTag("StageColl");

                    // OnCollisionEnter �Լ� ȣ��
                    OnCollisionEnter(&tempCollider);
                }
            }
            else if (pixel.r == 0 && pixel.g == 255 && pixel.b == 255) {
                if (GetCollider() && GetCollider()->GetColTag() != "Wall") {
                    GetCollider()->SetColTag("Wall");

                    // ���� CCollider ��ü ����
                    CCollider tempCollider;
                    tempCollider.SetColTag("Wall");

                    // OnCollisionEnter �Լ� ȣ��
                    OnWallCollisionEnter(&tempCollider);
                }
            }
            else {
                // �÷��̾ ����Ÿ ������ ��� ��� OnCollisionExit ȣ��
                if (GetCollider() && GetCollider()->GetColTag() == "StageColl") {
                    // ���� CCollider ��ü ����
                    CCollider tempCollider;
                    tempCollider.SetColTag("StageColl");

                    // OnCollisionExit �Լ� ȣ��
                    OnCollisionExit(&tempCollider);

                    // Collider �±� �ʱ�ȭ
                    GetCollider()->SetColTag("");
                }
                else if (GetCollider() && GetCollider()->GetColTag() == "Wall") {
                    // ���� CCollider ��ü ����
                    CCollider tempCollider;
                    tempCollider.SetColTag("Wall");

                    // OnCollisionExit �Լ� ȣ��
                    OnWallCollisionExit(&tempCollider);

                    // Collider �±� �ʱ�ȭ
                    GetCollider()->SetColTag("");
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

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageColl")
    {
        OutputDebugStringA("Magenta Pixel Colliders!\n");
        Vec2 vPos = GetPos();

        GetGravity()->SetOnGround(true);
    }
}

void CPlayer::OnCollision(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageColl")
    {
        OutputDebugStringA("Magenta Pixel Collidering!\n");
        Vec2 vPos = GetPos();

        GetGravity()->SetOnGround(true);
    }

}

void CPlayer::OnCollisionExit(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "StageColl")
    {
        OutputDebugStringA("Collision Exit!\n");
        GetGravity()->SetOnGround(false);
    }
}

void CPlayer::OnWallCollisionEnter(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "Wall")
    {
        OutputDebugStringA("Wall Collider\n");
        m_bIsOnWall = true; // Wall �浹 ���� ����

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
}

void CPlayer::OnWallCollisionExit(CCollider* _pOther)
{
    if (_pOther->GetColTag() == "Wall")
        {
        OutputDebugStringA("Wall Collision Exit\n");
        m_bIsOnWall = false; // Wall �浹 ���� ����

        // StageColl �浹 ���ο� ������� m_bLeftEnable�� m_bRightEnable�� true�� ����
        m_bLeftEnable = true;
        m_bRightEnable = true;
    }
}

void CPlayer::Update()
{
    Update_State();
    Update_Move();
    Update_Animation();

    CheckPixelColor();

    m_ePrevState = m_eCurState;
    m_iPrevDir = m_iDir;
}

void CPlayer::Render(HDC _dc)
{
    CObject::ComponentRender(_dc);

    Vec2 vPos = GetPos();
    vPos = CCamera::GetInst()->GetRenderPos(vPos);

    // ���� ���¿� ������ ȭ�鿡 ���
    wchar_t strState[100];
    swprintf_s(strState, L"Player State: %d", static_cast<int>(m_eCurState));
    TextOut(_dc, 200, 20, strState, lstrlen(strState));

    // m_bLeftEnable�� m_bRightEnable�� ���� ȭ�鿡 ���
    wchar_t strLeftEnable[100];
    swprintf_s(strLeftEnable, L"Left Enable: %s", m_bLeftEnable ? L"true" : L"false");
    TextOut(_dc, 200, 40, strLeftEnable, lstrlen(strLeftEnable));

    wchar_t strRightEnable[100];
    swprintf_s(strRightEnable, L"Right Enable: %s", m_bRightEnable ? L"true" : L"false");
    TextOut(_dc, 200, 60, strRightEnable, lstrlen(strRightEnable));
}