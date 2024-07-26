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

    Vec2 vPos = GetPos();
    vPos = CCamera::GetInst()->GetRenderPos(vPos);

    // µ·ÀÇ ·»´õ¸µ ·ÎÁ÷ Ãß°¡
    Ellipse(_dc, vPos.x - 10, vPos.y - 10, vPos.x + 10, vPos.y + 10);
}