#include "../pch.h"
#include "CMeso.h"
#include "../Module/SelectGDI.h"
#include "../Component/CCollider.h"
#include "../Resource/CTexture.h"
#include "../Manager/CResourceManager.h"

CMeso::CMeso() :
	CItem(),
	m_iMoneyAmount(0)
{
    m_pTex = CResourceManager::GetInst()->LoadTexture(L"Meso", L"texture\\Meso.bmp");

    AddCollider();
    auto moneyCollider = GetCollider().back();
    moneyCollider->SetColTag("Meso");
    moneyCollider->SetOffsetPos(Vec2{ 0.f, 0.f });
    moneyCollider->SetScale(Vec2{ 30.f, 30.f });
    moneyCollider->SetLayer(1);
}

CMeso::~CMeso()
{
}

void CMeso::Update()
{
}

void CMeso::Render(HDC _dc)
{
    CObject::ComponentRender(_dc);

    int iWidth = (int)m_pTex->Width();
    int iHeight = (int)m_pTex->Height();

    Vec2 vPos = GetPos();
    vPos = CCamera::GetInst()->GetRenderPos(vPos);

    // µ·ÀÇ ·»´õ¸µ ·ÎÁ÷ Ãß°¡
    TransparentBlt(_dc,
        int(vPos.x - (float)(iWidth / 2)),
        int(vPos.y - (float)(iHeight / 2)),
        iWidth, iHeight,
        m_pTex->GetDC(),
        0, 0, iWidth, iHeight,
        RGB(255, 255, 255));
}