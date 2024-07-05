#include "../pch.h"
#include "CMeso.h"
#include "../Module/SelectGDI.h"
#include "../Component/CCollider.h"

CMeso::CMeso() :
	CItem(),
	m_iMoneyAmount(0)
{
    AddCollider();
    auto moneyCollider = GetCollider().back();
    moneyCollider->SetColTag("Meso");
    moneyCollider->SetOffsetPos(Vec2{ 0.f, 0.f });
    moneyCollider->SetScale(Vec2{ 20.f, 20.f });
    moneyCollider->SetLayer(1);
}

CMeso::~CMeso()
{
}

void CMeso::Update()
{
}

void CMeso::FinalUpdate()
{
}

void CMeso::Render(HDC _dc)
{
    CObject::ComponentRender(_dc);

    Vec2 vPos = GetItemInfo();
    vPos = CCamera::GetInst()->GetRenderPos(vPos);

    // µ·ÀÇ ·»´õ¸µ ·ÎÁ÷ Ãß°¡
    Ellipse(_dc, vPos.x - 10, vPos.y - 10, vPos.x + 10, vPos.y + 10);
}